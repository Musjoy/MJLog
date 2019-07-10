
//
//  MJLog.m
//  iOSDemoXlog
//
//  Created by xd on 2019/5/31.
//

#import "MJLog.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <sys/xattr.h>

#import <mars/xlog/xlogger.h>
#import <mars/xlog/xloggerbase.h>
#import <mars/xlog/appender.h>
#import <SSZipArchive/SSZipArchive.h>

#import <ModuleCapability/ModuleCapability.h>
#ifdef HEADER_CONTROLLER_MANAGER
#import HEADER_CONTROLLER_MANAGER
#endif

static NSString *kLogPath = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0] stringByAppendingString:@"/log"];

static NSString *const kMJLogConfigKey = @"com.mjlog.config.main";
static NSString *const kMJLogWriteEnable = @"com.mjlog.config.write";
static NSString *const kMJLogConsoleLogEnable = @"com.mjlog.config.consolelog";

@implementation MJLog
+ (instancetype)shard {
    static MJLog *_manager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _manager = [[self alloc] init];
    });
    
    return _manager;
}

/// xlog默认清除本目录10天以上的文件
- (instancetype)init {
    self = [super init];
    if (self) {
        [self logOpen];
    }
    return self;
}

#pragma mark - Base
- (void)setWriteToFile:(BOOL)writeToFile {
    if ((NO == writeToFile) && (YES == _writeToFile)) {
        /// 关闭log，先把未写入的文件写到文件
        [self sync];
    }
    
    _writeToFile = writeToFile;
    [self updateConfig];
}

- (void)setConsoleLogInRelease:(BOOL)consoleLogInRelease {
    _consoleLogInRelease = consoleLogInRelease;
#if DEBUG
    appender_set_console_log(true);
#else
    appender_set_console_log(_consoleLogInRelease);
#endif
    [self updateConfig];
}

- (void)updateConfig {
    NSDictionary *config = @{kMJLogWriteEnable : @(_writeToFile),
                             kMJLogConsoleLogEnable : @(_consoleLogInRelease)
                             };
    [[NSUserDefaults standardUserDefaults] setObject:config forKey:kMJLogConfigKey];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (void)loadConfig {
    _writeToFile = NO;
    _consoleLogInRelease = NO;
    
    NSDictionary *config = [[NSUserDefaults standardUserDefaults] objectForKey:kMJLogConfigKey];
    if (config) {
        _writeToFile = config[kMJLogWriteEnable] ? [config[kMJLogWriteEnable] boolValue] : NO;
        _consoleLogInRelease = config[kMJLogConsoleLogEnable] ? [config[kMJLogConsoleLogEnable] boolValue] : NO;
    }
}

- (void)logWithLevel:(NSString *)level line:(int)line file:(const char*)file func:(const char*)func content:(NSString *)content {
    XLoggerInfo info;
    info.level = kLevelInfo;
    info.func_name = func;
    info.filename = file;
    info.line = line;
    info.tag = [level UTF8String];
    gettimeofday(&info.timeval, NULL);
    info.tid = (uintptr_t)[NSThread currentThread];
    info.maintid = (uintptr_t)[NSThread mainThread];
    info.pid = 0;
    
    if (_writeToFile) {
        /// 打印并写入到文件
        xlogger_Write(&info, content.UTF8String);
    } else {
        /// 只打印
        NSString *funcStr = [NSString stringWithUTF8String:func];
#if DEBUG
        /// 做区别处理是因为默认log组件有2种使用情形，log种带有代码位置和不带有位置两种情况
        if (strlen(func) > 0) {
            NSLog(@"[%@] %@[Line %d]%@",level, funcStr, line, content);
        } else {
            NSLog(@"[%@]%@", level, content);
        }
#else
        if (_consoleLogInRelease) {
            if (strlen(func) > 0) {
                NSLog(@"[%@] %@[Line %d]%@",level, funcStr, line, content);
            } else {
                NSLog(@"[%@]%@", level, content);
            }
        }
#endif
    }
}

- (void)logOpen {
    [self loadConfig];
    NSString* logPath = kLogPath;
    
    // set do not backup for logpath
    const char* attrName = "com.apple.MobileBackup";
    u_int8_t attrValue = 1;
    setxattr([logPath UTF8String], attrName, &attrValue, sizeof(attrValue), 0, 0);
    
    // init xlog
#if DEBUG
    xlogger_SetLevel(kLevelDebug);
    appender_set_console_log(true);
#else
    xlogger_SetLevel(kLevelInfo);
    appender_set_console_log(_consoleLogInRelease);
#endif
    appender_open(kAppednerAsync, [logPath UTF8String], "MJLog", "");
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(applicationWillTerminate) name:UIApplicationWillTerminateNotification object:nil];
}

- (void)logClose {
    appender_flush_sync();
    appender_close();
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)sync {
    appender_flush_sync();
}

- (void)applicationWillTerminate {
    [self logClose];
}

- (void)dealloc {
    [self logClose];
}

#pragma mark - Transfer
/// 获取近几天的log文件
+ (NSArray *)currentLogPathWithDays:(NSUInteger)days {
    /// 将当前的内存映射文件中的log回写到磁盘
    [[MJLog shard] sync];
    
    NSString *targetDay = [self getTargetLogNameWithdays:days];
    NSString* logPath = kLogPath;
    NSArray *arrLogs = [[NSFileManager defaultManager] subpathsAtPath:logPath];
    
    NSMutableArray *retArr = [NSMutableArray array];
    for (NSString *path in arrLogs) {
        /// 过滤掉内存映射的临时文件，只获取log文件
        if ([path hasSuffix:@"xlog"]) {
            if ([targetDay compare:path] != NSOrderedDescending) {
                NSString *newPath = [logPath stringByAppendingPathComponent:path];
                [retArr addObject:newPath];
            }
        }
    }
    
    NSArray *ret = [retArr.copy sortedArrayUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
        return [obj1 compare:obj2];
    }];
    
    return ret;
}

+ (NSString *)getTargetLogNameWithdays:(NSUInteger)days {
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyyMMdd"];
    NSTimeInterval timeTarget = [[NSDate date] timeIntervalSince1970] - 86400 * (days - 1);
    NSDate *targetDate = [NSDate dateWithTimeIntervalSince1970:timeTarget];
    NSString *dateStr = [formatter stringFromDate:targetDate];
    NSString *targetDay = [NSString stringWithFormat:@"MJLog_%@.xlog", dateStr];
    
    return targetDay;
}

/// 需要获取的log一起打包为一个zip，文件名是日期起止时间
+ (NSURL *)getZipFileUrlWithLogs:(NSArray *)logs {
    if (0 == logs.count) {
        return nil;
    }
    
    /// 移除旧的zip
    NSString* logPath = kLogPath;
    NSArray *arrLogs = [[NSFileManager defaultManager] subpathsAtPath:logPath];
    for (NSString *path in arrLogs) {
        if ([path hasSuffix:@"zip"]) {
            NSString *oldZipPath = [logPath stringByAppendingPathComponent:path];
            [[NSFileManager defaultManager] removeItemAtPath:oldZipPath error:nil];
        }
    }

    /// 创建zip文件
    NSString *zipPath = [self zipPathWithLogs:logs];
    BOOL zipSuc = [SSZipArchive createZipFileAtPath:zipPath withFilesAtPaths:logs];
    if (!zipSuc) {
        [[NSFileManager defaultManager] removeItemAtPath:zipPath error:nil];
        return nil;
    }
    
    return [NSURL fileURLWithPath:zipPath];
}

+ (NSString *)zipPathWithLogs:(NSArray *)logs {
    if (1 == logs.count) {
        NSString *logPath = [logs[0] lastPathComponent];
        NSString *zipPath = [logPath stringByReplacingOccurrencesOfString:@"xlog" withString:@"zip"];
        zipPath = [kLogPath stringByAppendingPathComponent:zipPath];
        return zipPath;
    }
    
    /// MJLog_20190704.xlog
    NSString *firstLogPath = [logs[0] lastPathComponent];
    NSString *lastLogPath = [[logs lastObject] lastPathComponent];
    
    /// MJLog_20190704
    firstLogPath = [firstLogPath stringByReplacingOccurrencesOfString:@".xlog" withString:@""];
    
    /// _20190705
    lastLogPath = [[lastLogPath stringByReplacingOccurrencesOfString:@".xlog" withString:@""] stringByReplacingOccurrencesOfString:@"MJLog" withString:@""];
    
    /// MJLog_20190704_20190705.zip
    NSString *path = [NSString stringWithFormat:@"%@%@.zip" ,firstLogPath ,lastLogPath];
    path = [kLogPath stringByAppendingPathComponent:path];
    
    return path;
}

+ (UIViewController *)topViewController
{
#ifdef MODULE_CONTROLLER_MANAGER
    return [MJControllerManager topNavViewController];
#else
    UIViewController *topVC = nil;
    
    // Find the top window (that is not an alert view or other window)
    UIWindow *topWindow = [[UIApplication sharedApplication] keyWindow];
    if (topWindow.windowLevel != UIWindowLevelNormal) {
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for(topWindow in windows) {
            if (topWindow.windowLevel == UIWindowLevelNormal)
                break;
        }
    }
    
    UIView *rootView = [[topWindow subviews] objectAtIndex:0];
    id nextResponder = [rootView nextResponder];
    
    if ([nextResponder isKindOfClass:[UIViewController class]]) {
        topVC = nextResponder;
    } else if ([topWindow respondsToSelector:@selector(rootViewController)] && topWindow.rootViewController != nil) {
        topVC = topWindow.rootViewController;
    } else {
        NSAssert(NO, @"Could not find a root view controller.");
    }
    
    UIViewController *presentVC = topVC.presentedViewController;
    while (presentVC) {
        topVC = presentVC;
        presentVC = topVC.presentedViewController;
    }
    return topVC;
#endif
}

#pragma mark - Share
+ (BOOL)shareLogFileWithDays:(NSUInteger)days {
    if (days > 10) {
        days = 10;
    } else if (0 == days) {
        days = 1;
    }
    
    NSURL *zipUrl = [self getZipFileUrlWithLogs:[self currentLogPathWithDays:days]];
    if (!zipUrl) {
        return NO;
    }
    
    UIActivityViewController *activituVC = [[UIActivityViewController alloc]initWithActivityItems:@[zipUrl] applicationActivities:nil];
    [[self topViewController] presentViewController:activituVC animated:YES completion:nil];
    
    return YES;
}

#pragma mark - Localize

NSString * MJLogLocalizedString(NSString *key)
{
    return [[MJLog bundleForStrings] localizedStringForKey:key value:key table:@"MJLogStrings"];
}

+ (NSBundle *)bundleForStrings
{
    static NSBundle *bundle;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        NSBundle *bundleForClass = [NSBundle bundleForClass:[self class]];
        NSString *stringsBundlePath = [bundleForClass pathForResource:@"MJLogStrings" ofType:@"bundle"];
        bundle = [NSBundle bundleWithPath:stringsBundlePath] ?: bundleForClass;
    });
    
    return bundle;
}

@end
