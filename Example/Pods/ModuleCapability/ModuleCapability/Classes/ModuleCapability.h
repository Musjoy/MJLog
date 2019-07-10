//
//  ModuleCapability.h
//  Common
//
//  Created by 黄磊 on 16/4/14.
//  Copyright © 2016年 Musjoy. All rights reserved.
//

#ifndef ModuleCapability_h
#define ModuleCapability_h

#ifdef __cplusplus
#define MJ_EXTERN_C_BEGIN  extern "C" {
#define MJ_EXTERN_C_END  }
#else
#define MJ_EXTERN_C_BEGIN
#define MJ_EXTERN_C_END
#endif

#ifdef __cplusplus
# define MJ_EXTERN extern "C" __attribute__((visibility("default")))
#else
# define MJ_EXTERN extern __attribute__((visibility("default")))
#endif


//######################################
#pragma mark - 常用函数
//######################################
// 字符串拼接
#define combine(x, y)   [(x) stringByAppendingString:(y)]
#define combinePath(x, y)   [(x) stringByAppendingPathComponent:(y)]
// 字符串长度获取
#define textSizeWithFont(text, font) ([text length] > 0 ? [text sizeWithAttributes:@{NSFontAttributeName:font}] : CGSizeZero);
#define multilineTextSize(text, font, maxSize) ([text length] > 0 ? \
[text boundingRectWithSize:maxSize \
options:(NSStringDrawingUsesFontLeading | NSStringDrawingUsesLineFragmentOrigin) \
attributes:@{NSFontAttributeName:font} \
context:nil].size : CGSizeZero);


//######################################
#pragma mark - 用户自定义常量导入
//######################################
#if __has_include("Constant.h")
#import "Constant.h"
#endif



//######################################
#pragma mark - 部分常量定义
//######################################
// 公开版本号。eg：1.0
#define kClientVersionShort [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"]
// 内部版本号。eg：1.0.1
#define kClientVersion [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"]
// App Store
#ifndef kAppID
#define kAppID   @"unknown"
#ifdef IN_PROJECT
#ifdef DEBUG
#warning    @"App id is not defined!"
#else
#error      @"App id is not defined!"
#endif
#endif
#endif
// 当前iOS版本
#define __CUR_IOS_VERSION   ([[[UIDevice currentDevice] systemVersion] floatValue] * 10000)
#define kAppLookUpUrl       combine(@"https://itunes.apple.com/lookup?id=", kAppID)
#define kAppDownload        combine(@"https://itunes.apple.com/app/id", kAppID)
#define kAppOldComment      combine(combine(@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=", kAppID), @"&pageNumber=0&sortOrdering=2&type=Purple+Software&mt=8")
#define kAppComment         ((__CUR_IOS_VERSION<110000)?kAppOldComment:kAppDownload)
#define kAppBundleId        [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"]

// 屏幕高度
#define kScreenHeight [UIScreen mainScreen].bounds.size.height
// 屏幕宽度
#define kScreenWidth  [UIScreen mainScreen].bounds.size.width


//######################################
#pragma mark - 常量默认值
//######################################
/** 默认动画持续时间 */
#ifndef DEFAULT_ANIMATE_DURATION
#define DEFAULT_ANIMATE_DURATION 0.3
#endif


//######################################
#pragma mark - Logging配置
//######################################
#ifndef LOGGING_CONFIG_BY_USER
#if defined(DEBUG) || defined(ForTest)
// TRACE CONFIG
#define LOGGING_ENABLED 1
#define LOGGING_INCLUDE_CODE_LOCATION 1
#define LOGGING_LEVEL_INFO 1
// FILE LOGGING
//#define LOGGING_TO_FILE
#endif
#endif
#import "Logging.h"


//######################################
#pragma mark - 本地化
//######################################

#ifndef locString
#ifdef MODULE_LOCALIZE
#define HEADER_LOCALIZE     <MJLocalize/MJLocalize.h>
#define locString(str)                  [MJLocalize localizedString:str]
#define locStringWithFormat(str,...)    [MJLocalize localizedStringWithFormat:str, __VA_ARGS__]
#define locStringWithArgs(str, arrArgs) [MJLocalize localizedString:str withArguments:arrArgs]
#else
#define HEADER_LOCALIZE     <Foundation/Foundation.h>
#define locString(str)                  NSLocalizedString(str, nil)
#define locStringWithFormat(str,...)    [NSString localizedStringWithFormat:NSLocalizedString(str,nil), __VA_ARGS__]
#define locStringWithArgs(str, arrArgs) ({  \
NSString *strLocalized = NSLocalizedString(str, nil);       \
if (arrArgs && arrArgs.count > 0) {         \
    for (NSInteger i, len=arrArgs.count; i < len; i++) {    \
        strLocalized = [strLocalized stringByReplacingOccurrencesOfString:[NSString stringWithFormat:@"{%d}", i]    \
                                                               withString:arrArgs[i]];     \
    }                                       \
}                                           \
strLocalized;                               \
})
#endif
#else 
#define HEADER_LOCALIZE     <Foundation/Foundation.h>
#endif

//######################################
#pragma mark - Json 解析
//######################################
// Json 解析
#ifdef MODULE_DB_MODEL
#define HEADER_MODEL        <DBModel/DBModel.h>
#define HEADER_JSON_PARSE   <DBModel/DBModel.h>
#define MODEL_BASE_CLASS    DBModel
#define objectFromString(str, err) [DBModel objectFromJSONString:str error:err]
#else
#define HEADER_MODEL        <Foundation/Foundation.h>
#define HEADER_JSON_PARSE   <Foundation/Foundation.h>
#define MODEL_BASE_CLASS    NSObject
#define objectFromString(str, err) ({       \
    id aNil = nil;                          \
    id obj = aNil;                          \
    NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];            \
    if (data) {                             \
        NSError *initError = aNil;          \
        @try {                              \
            obj = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&initError];   \
        } @catch (NSException *exception) { \
            obj = aNil;                     \
        }                                   \
    }                                       \
    obj;                                    \
})
#endif
// Json 生成
#ifdef MODULE_UTILS
#define HEADER_JSON_GENERATE    <MJUtils/NSDictionary+Utils.h>
#define jsonStringFromDic(aDic) [aDic convertToJsonString]
#else
#define HEADER_JSON_GENERATE    <Foundation/Foundation.h>
#define jsonStringFromDic(aDic) ({          \
    NSData* jsonData = nil;                 \
    NSError* jsonError = nil;               \
    @try {                                  \
        jsonData = [NSJSONSerialization dataWithJSONObject:aDic             \
                                                   options:kNilOptions      \
                                                     error:&jsonError];     \
    } @catch (NSException *exception) {     \
        jsonData = nil;                     \
    }                                       \
    [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding]; \
})
#endif


//######################################
#pragma mark - 服务器URL
//######################################
#ifndef HEADER_SERVER_URL
#define HEADER_SERVER_URL   <Foundation/Foundation.h>
#endif

//######################################
#pragma mark - 网络请求服务
//######################################
#ifndef HEADER_WEB_SERVICE
// callback (^(NSURLResponse *response, id data, NSError *error) )
#ifdef MODULE_WEB_SERVICE
#define HEADER_WEB_SERVICE  <MJWebService/MJWebService.h>
#define getServerUrl(urlString, callback) [MJWebService startGet:urlString body:nil completion:callback]
#else
#define HEADER_WEB_SERVICE  <Foundation/Foundation.h>
#define getServerUrl(urlString, callback) { \
NSOperationQueue *queue = [[NSOperationQueue alloc] init]; \
[NSURLConnection sendAsynchronousRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:urlString]] \
                                   queue:queue \
                       completionHandler:^(NSURLResponse *response, NSData *data, NSError *connectionError) { \
                            id aNil = nil; \
                            id aData = aNil; \
                            if (data) { \
                                NSError *initError = aNil; \
                                @try { \
                                    aData = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&initError]; \
                                } @catch (NSException *exception) { \
                                    aData = aNil;                     \
                                } \
                                if (!aData) { \
                                    aData = [[NSString alloc] initWithData:data encoding:4]; \
                                } \
                            } \
                            void (^aCallback)(NSURLResponse *response, id data, NSError *error) = callback; \
                            aCallback ? aCallback(response, aData, connectionError) : 0; \
                       }];}
#endif
#endif

//######################################
#pragma mark - 统计分析模块
//######################################
#ifndef DEBUG
#if __has_include(<UMMobClick/MobClick.h>)
#define HEADER_ANALYSE  <UMMobClick/MobClick.h>
#define triggerEventStr(eventId, evenDesc) [MobClick event:eventId attributes:@{@"name":evenDesc}]
#define triggerEvent(eventId, attrs) [MobClick event:eventId attributes:attrs]
#define triggerBeginPage(className) [MobClick beginLogPageView:className]
#define triggerEndPage(className) [MobClick endLogPageView:className]
#endif
#endif
#ifndef triggerEventStr
#define HEADER_ANALYSE  <Foundation/Foundation.h>
#define triggerEventStr(eventId, evenDesc) NSLog(@"Event trigger : %@", evenDesc)
#define triggerEvent(eventId, attrs) NSLog(@"Event trigger : %@ ; %@", attrs[@"name"], attrs)
#define triggerBeginPage(className) NSLog(@"Page begin : %@", className)
#define triggerEndPage(className) NSLog(@"Page end : %@", className)
#endif
/// 对应通用统计事件定义
#ifndef STAT_Error
#define STAT_Error      @"Error"
#endif


//######################################
#pragma mark - FileSource
//######################################
#ifdef MODULE_FILE_SOURCE
#define HEADER_FILE_SOURCE  <FileSource/FileSource.h>
#define getFileData(aFileName) [FileSource dataWithFileName:aFileName]
#define getPlistFileData(aPlistName) [FileSource dataWithPlistName:aPlistName]
#define getJsonFileData(aJsonName) [FileSource dataWithJsonName:aJsonName]
#else
#define HEADER_FILE_SOURCE  <Foundation/Foundation.h>
#define getPlistFileData(aPlistName) ({ \
    NSString *fileName = [aPlistName stringByAppendingString:@".plist"]; \
    NSString *fileBundle = [[NSBundle mainBundle] resourcePath]; \
    NSString *filePath = [fileBundle stringByAppendingPathComponent:fileName]; \
    id aDic = [[NSDictionary alloc] initWithContentsOfFile:filePath]; \
    aDic; \
})
#define getJsonFileData(aJsonName) ({ \
    NSString *fileName = [aJsonName stringByAppendingString:@".json"]; \
    NSString *fileBundle = [[NSBundle mainBundle] resourcePath]; \
    NSString *filePath = [fileBundle stringByAppendingPathComponent:fileName]; \
    NSData *data=[NSData dataWithContentsOfFile:filePath];             \
    id aNil = nil;  \
    id jsonObject=[NSJSONSerialization JSONObjectWithData:data          \
                                                  options:NSJSONReadingAllowFragments   \
                                                    error:nil];          \
    jsonObject; \
})
#define getFileData(aFileName) getPlistFileData(aFileName)
#endif

//######################################
#pragma mark - ControllerManager
//######################################
// BaseViewController
#if __has_include("BaseViewController.h")
#define HEADER_BASE_VIEW_CONTROLLER "BaseViewController.h"
#define THEBaseViewController       BaseViewController
#elif (defined(MODULE_CONTROLLER_MANAGER) || __has_include(<MJControllerManager/MJBaseViewController.h>))
#define HEADER_BASE_VIEW_CONTROLLER <MJControllerManager/MJBaseViewController.h>
#define THEBaseViewController       MJBaseViewController
#else
#define HEADER_BASE_VIEW_CONTROLLER <UIKit/UIKit.h>
#define THEBaseViewController       UIViewController
#endif
// NavigationController
#if __has_include("NavigationController.h")
#define HEADER_NAVIGATION_CONTROLLER    "NavigationController.h"
#define THENavigationController         NavigationController
#elif (defined(MODULE_CONTROLLER_MANAGER) || __has_include(<MJControllerManager/MJNavigationController.h>))
#define HEADER_NAVIGATION_CONTROLLER    <MJControllerManager/MJNavigationController.h>
#define THENavigationController         MJNavigationController
#else
#define HEADER_NAVIGATION_CONTROLLER    <UIKit/UINavigationController.h>
#define THENavigationController         UINavigationController
#endif
// ControllerManager
#if __has_include("ControllerManager.h")
#define HEADER_CONTROLLER_MANAGER   "ControllerManager.h"
#define THEControllerManager        ControllerManager
#elif (defined(MODULE_CONTROLLER_MANAGER) || __has_include(<MJControllerManager/MJControllerManager.h>))
#define HEADER_CONTROLLER_MANAGER   <MJControllerManager/MJControllerManager.h>
#define THEControllerManager        MJControllerManager
#endif
// WindowRootViewController
#if __has_include(<MJControllerManager/MJWindowRootViewController.h>)
#define HEADER_WINDOW_ROOT_VIEW_CONTROLLER  <MJControllerManager/MJWindowRootViewController.h>
#define THEWindowRootViewController         MJWindowRootViewController
#else
#define HEADER_WINDOW_ROOT_VIEW_CONTROLLER  HEADER_BASE_VIEW_CONTROLLER
#define THEWindowRootViewController         THEBaseViewController
#endif
// ControllerUtil
#ifndef HEADER_CONTROLLER_UTIL
#ifdef  MODULE_CONTROLLER_MANAGER
#define HEADER_CONTROLLER_UTIL      HEADER_CONTROLLER_MANAGER
#define getTopContainerController() [THEControllerManager topContainerController]
#else
#define HEADER_CONTROLLER_UTIL      <UIKit/UIKit.h>
#define getTopContainerController() ({ \
UIViewController *topVC = nil; \
topVC = [[[UIApplication sharedApplication] keyWindow] rootViewController]; \
UIViewController *presentVC = topVC.presentedViewController; \
while (presentVC) { \
    topVC = presentVC; \
    presentVC = topVC.presentedViewController; \
} \
topVC; \
})
#endif
#endif

//######################################
#pragma mark - Keychain
//######################################
#ifndef HEADER_KEYCHAIN
#ifdef  MODULE_KEYCHAIN
#define HEADER_KEYCHAIN <MJKeychain/MJKeychain.h>
#define keychainSetDefaultObject(obj, key)  [[MJKeychain defaultKeychain] setObject:obj forKey:key]
#define keychainDefaultObjectForKey(key)    [[MJKeychain defaultKeychain] objectForKey:key]
#define keychainSetDefaultSharedObject(obj, key)  [[MJKeychain defaultSharedKeychain] setObject:obj forKey:key]
#define keychainDefaultSharedObjectForKey(key)    [[MJKeychain defaultSharedKeychain] objectForKey:key]
#else
#define HEADER_KEYCHAIN <Foundation/Foundation.h>
#define keychainSetDefaultObject(obj, key)  ({ \
NSMutableDictionary *dicQuery = [NSMutableDictionary dictionaryWithObjectsAndKeys:(id)kSecClassGenericPassword, kSecClass, kCFBooleanTrue, kSecReturnAttributes, [[[NSBundle mainBundle] bundleIdentifier] stringByAppendingString:@".DefaultService"], kSecAttrService, key, kSecAttrAccount, nil]; \
CFDictionaryRef cfResult; \
if (SecItemCopyMatching((CFDictionaryRef)dicQuery, (CFTypeRef *)&cfResult) == noErr) { \
NSDictionary *dicUpdate = [NSDictionary dictionaryWithObjectsAndKeys:obj, kSecAttrGeneric, nil]; \
SecItemUpdate((CFDictionaryRef)dicQuery, (CFDictionaryRef)dicUpdate); \
} else { \
[dicQuery setObject:obj forKey:(id)kSecAttrGeneric]; \
SecItemAdd((CFDictionaryRef)dicQuery, (CFTypeRef *)&cfResult); \
} \
})
#define keychainDefaultObjectForKey(key)    ({ \
NSMutableDictionary *dicQuery = [NSMutableDictionary dictionaryWithObjectsAndKeys:(id)kSecClassGenericPassword, kSecClass, kCFBooleanTrue, kSecReturnAttributes, [[[NSBundle mainBundle] bundleIdentifier] stringByAppendingString:@".DefaultService"], kSecAttrService, key, kSecAttrAccount, nil]; \
CFDictionaryRef cfResult; \
id object = nil; \
if (SecItemCopyMatching((CFDictionaryRef)dicQuery, (CFTypeRef *)&cfResult) == noErr) { \
NSDictionary *attributes = (__bridge NSDictionary*)cfResult; \
object = [attributes objectForKey:(id)kSecAttrGeneric]; \
} \
object; \
})
#define keychainSetDefaultSharedObject(obj, key)  keychainSetDefaultObject(obj, key)
#define keychainDefaultSharedObjectForKey(key)    keychainDefaultObjectForKey(key)
#endif
#endif


//######################################
#pragma mark - Alert
//######################################
#ifndef HEADER_ALERT
#ifdef  MODULE_ALERT_MANAGER
#define HEADER_ALERT <MJAlertManager/MJAlertManager.h>
#define alertMessage(msg)                               [MJAlertManager showAlertWithTitle:nil message:msg]
#define alertTitleMessage(title, msg)                   [MJAlertManager showAlertWithTitle:title message:msg]
#define alertTitleMessageCallback(title, msg, callback) [MJAlertManager showAlertWithTitle:title message:msg completion:callback]
#define alertInfoCallback(alertInfo, callback)          [MJAlertManager showAlertWith:alertInfo completion:callback]
#define alertNoBtnsTitleMessage(title, msg)             [MJAlertManager showAlertNoBtnsWithTitle:title message:msg]
#define alertRefreshTitleMessage(alert, aTitle, msg)    [MJAlertManager refreshAlert:alert title:aTitle message:msg];
#define alertClickButtonAtIndex(alert, aIndex)          [MJAlertManager clickAlert:alert atIndex:aIndex]
#define alertTitleMessageButtonsCallback(title, msg, cancl, confm, destry, callback)        [MJAlertManager showAlertWithTitle:title message:msg cancel:cancl confirm:confm destroy:destry otherButtons:nil completion:callback]
#else
#define HEADER_ALERT <UIKit/UIKit.h>
#define alertMessage(msg)                               alertTitleMessage(nil, msg)
#define alertTitleMessage(title, msg)                   ({ \
UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:title message:msg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil]; \
[alertView show]; \
alertView; \
})
#define alertTitleMessageCallback(title, msg, callback) alertTitleMessageButtonsCallback(title, msg, nil, @"OK", nil, callback)
#define alertInfoCallback(alertInfo, callback)          alertTitleMessageButtonsCallback([alertInfo objectForKey:@"title"], [alertInfo objectForKey:@"message"], [alertInfo objectForKey:@"cancel"], [alertInfo objectForKey:@"confirm"], [alertInfo objectForKey:@"destroy"], callback)
#define alertNoBtnsTitleMessage(title, msg)             ({ \
UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:title message:msg delegate:nil cancelButtonTitle:nil otherButtonTitles:nil]; \
[alertView show]; \
alertView; \
})
#define alertRefreshTitleMessage(alert, aTitle, msg)    ({ \
alert.title = aTitle; \
alert.message = msg; \
})
// 这里暂时无法支持
#define alertClickButtonAtIndex(alert, aIndex)          ({ \
})
#define alertTitleMessageButtonsCallback(title, msg, cancel, confirm, destroy, callback)    ({ \
UIAlertController *alertVC = [UIAlertController alertControllerWithTitle:title message:msg preferredStyle:UIAlertControllerStyleAlert]; \
void (^alertClick)(NSInteger ) = ^(NSInteger selectIndex) { \
    callback ? callback(selectIndex) : 0; \
}; \
if (cancel != nil) { \
    [alertVC addAction:[UIAlertAction actionWithTitle:cancel style:(UIAlertActionStyleCancel) handler:^(UIAlertAction * _Nonnull action) { \
        alertClick(0); \
    }]]; \
} \
if (confirm != nil) { \
    [alertVC addAction:[UIAlertAction actionWithTitle:confirm style:(UIAlertActionStyleDefault) handler:^(UIAlertAction * _Nonnull action) { \
        alertClick(1); \
    }]]; \
} \
if (destroy != nil) { \
    [alertVC addAction:[UIAlertAction actionWithTitle:destroy style:(UIAlertActionStyleDestructive) handler:^(UIAlertAction * _Nonnull action) { \
        alertClick(-1); \
    }]]; \
} \
UIViewController *topVC = getTopContainerController(); \
[topVC presentViewController:alertVC animated:YES completion:NULL]; \
})


#endif
#endif

#endif /* ModuleCapability_h */
