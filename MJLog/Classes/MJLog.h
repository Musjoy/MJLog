//
//  MJLog.h
//  iOSDemoXlog
//
//  Created by xd on 2019/5/31.
//

/*
 log写入，存储使用微信xlog组件:https://github.com/Tencent/mars , 使用内存映射方式回写log到磁盘，避免了卡顿和log丢失情况。 log文件每天存储一个文件，默认保留近10天的log（保留时间和文件按大小分割可配置调整）
 */


#import <Foundation/Foundation.h>
#import "MJLogHandleViewController.h"

NS_ASSUME_NONNULL_BEGIN
@interface MJLog : NSObject
/// log是否写入到文件中，可随时开启/关闭，默认关闭
@property (nonatomic, assign) BOOL writeToFile;
/// release模式下log是否输出到控制台，默认关闭
@property (nonatomic, assign) BOOL consoleLogInRelease;
+ (instancetype)shard;

/**
 分享log文件
 
 @param days 最近多少天内的文件，1为当天，3为近3天，依此类推
 @return log不存在或者压缩失败则返回NO，否则返回YES
 */
+ (BOOL)shareLogFileWithDays:(NSUInteger)days;

/**
 与已有log组件对接，不建议手动调用，请使用LogTrace(), LogInfo()...

 @param level log级别，trace, info ...
 @param line 代码行数
 @param file 文件名
 @param func 方法名
 @param content log内容
 */
- (void)logWithLevel:(NSString *)level line:(int)line file:(const char*)file func:(const char*)func content:(NSString *)content;

/// 国际化
FOUNDATION_EXPORT NSString * MJLogLocalizedString(NSString *key);

@end

NS_ASSUME_NONNULL_END
