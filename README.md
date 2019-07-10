# MJLog

[![CI Status](https://img.shields.io/travis/XDD2333/MJLog.svg?style=flat)](https://travis-ci.org/XDD2333/MJLog)
[![Version](https://img.shields.io/cocoapods/v/MJLog.svg?style=flat)](https://cocoapods.org/pods/MJLog)
[![License](https://img.shields.io/cocoapods/l/MJLog.svg?style=flat)](https://cocoapods.org/pods/MJLog)
[![Platform](https://img.shields.io/cocoapods/p/MJLog.svg?style=flat)](https://cocoapods.org/pods/MJLog)

## Example

To run the example project, clone the repo, and run `pod install` from the Example directory first.

## Requirements

## Installation

MJLog is available through [CocoaPods](https://cocoapods.org). To install
it, simply add the following line to your Podfile:

```ruby
pod 'MJLog'
```

## Usage

- 日志打印及压缩，存储使用[微信xlog组件](https://mp.weixin.qq.com/s?__biz=MzA3NTYzODYzMg%3D%3D&mid=2653578220&idx=3&sn=5691bdd82ae0715ab12fd6b849f74aee&chksm=84b3b1ebb3c438fddf86bf74e232fa14222932ebd6d6439bed04ad17d5e64e9270d4ab460f64&scene=4)，使用内存映射方式写入log，避免频繁读写磁盘导致的性能问题，也保证了异常情况下log完整性
- 日志文件默认按天分割，每天一个文件（可调整为按大小分割）
- 默认保留log目录内10天内的文件（可调整），其他文件不要放到此目录，避免被自动清理。目录默认为Document/log/
- 分享log时，会将对应的所有log文件压缩为zip，调用系统分享发送文件。
- 解压缩后得到的后缀名为xlog的文件需要使用脚本解压，解压脚本在Resources目录下

```
/// 初始化, 在APPdelegate中尽量早调用
[MJLog shard];

/// 是否将log写入到文件，默认关闭，可随时开启关闭
[MJLog shard].writeToFile = YES;

/// release模式下是否将log打印到控制台，默认关闭，建议关闭
[MJLog shard].consoleLogInRelease = NO;

/// 分享日志方法1：直接调用此方法，参数days为代表获取最近多少天的日志，最多支持10天，如果返回NO则代表没有对应的日志文件，或者压缩为zip时失败，返回YES则自动调用系统分享功能分享日志zip文件
[MJLog shareLogFileWithDays:1];

/// 分享日志方法2：使用自带的分享界面，或自定义界面
MJLogHandleViewController *vc = [[MJLogHandleViewController alloc] init];
[self.navigationController pushViewController:vc animated:YES];

/// 解压缩日志文件
python decode_mars_nocrypt_log_file.py MJLog_20190704.xlog
```

## Author

XDD2333, dong.xia@musjoy.com

## License

MJLog is available under the MIT license. See the LICENSE file for more info.
