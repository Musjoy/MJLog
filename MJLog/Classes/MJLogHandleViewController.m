//
//  MJLogHandleViewController.m
//  MJLog_Example
//
//  Created by xd on 2019/7/5.
//  Copyright © 2019 XDD2333. All rights reserved.
//

typedef NS_ENUM(NSUInteger, MJLogUploadType) {
    MJLogUploadTypeToday = 1,
    MJLogUploadTypeLast3Days = 3,
    MJLogUploadTypeLast7Days = 7,
    MJLogUploadTypeLast10Days = 10, /// 默认最多10天
};

#import <UIKit/UIKit.h>
#import "MJLogHandleViewController.h"
#import "MJLog.h"

@interface MJLogHandleViewController ()<UIPickerViewDelegate, UIPickerViewDataSource>
@property (nonatomic, strong) UIPickerView *pickerView;
@property (nonatomic, assign) MJLogUploadType curType;
@property (nonatomic, strong) NSArray *arrData;
@end

@implementation MJLogHandleViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.title = MJLogLocalizedString(@"share_logs");
    
    self.curType = MJLogUploadTypeToday;
    self.arrData = @[@{@"title" : MJLogLocalizedString(@"today"), @"type" : @(MJLogUploadTypeToday)},
                     @{@"title" : MJLogLocalizedString(@"last_3_days"), @"type" : @(MJLogUploadTypeLast3Days)},
                     @{@"title" : MJLogLocalizedString(@"last_7_days"), @"type" : @(MJLogUploadTypeLast7Days)},
                     @{@"title" : MJLogLocalizedString(@"last_10_days"), @"type" : @(MJLogUploadTypeLast10Days)}];
    
    [self configUI];
    // Do any additional setup after loading the view.
}

- (void)configUI {
    UILabel *lblWrite = [[UILabel alloc] initWithFrame:CGRectMake(([UIScreen mainScreen].bounds.size.width - 240) / 2, 200, 180, 30)];
    lblWrite.font = [UIFont systemFontOfSize:20];
    lblWrite.text = MJLogLocalizedString(@"log_enable");
    lblWrite.textColor = [UIColor blueColor];
    [self.view addSubview:lblWrite];
    
    UISwitch *switchWrite = [[UISwitch alloc] initWithFrame:CGRectMake(CGRectGetMaxX(lblWrite.frame), CGRectGetMinY(lblWrite.frame), 50, 30)];
    switchWrite.on = [MJLog shard].writeToFile;
    [switchWrite addTarget:self action:@selector(switchWrite:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:switchWrite];
    
    self.pickerView = [[UIPickerView alloc] initWithFrame:CGRectMake(0, CGRectGetMaxY(lblWrite.frame) + 30, [UIScreen mainScreen].bounds.size.width, 40 * 5)];
    self.pickerView.delegate = self;
    self.pickerView.dataSource = self;
    [self.view addSubview:_pickerView];
    
    UIButton *btnShare = [UIButton buttonWithType:UIButtonTypeCustom];
    [btnShare setTitle:MJLogLocalizedString(@"share_logs") forState:UIControlStateNormal];
    [btnShare setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btnShare addTarget:self action:@selector(shareItem) forControlEvents:UIControlEventTouchUpInside];
    btnShare.frame = CGRectMake(([UIScreen mainScreen].bounds.size.width - 240) / 2, CGRectGetMaxY(_pickerView.frame) + 40, 240, 44);
    btnShare.backgroundColor = [UIColor blueColor];
    btnShare.layer.cornerRadius = 4;
    btnShare.clipsToBounds = YES;
    [self.view addSubview:btnShare];
    
    [self.pickerView reloadAllComponents];
}

- (void)switchWrite:(UISwitch *)sender {
    [MJLog shard].writeToFile = sender.isOn;
}

- (void)switchConsole:(UISwitch *)sender {
    [MJLog shard].consoleLogInRelease = sender.isOn;
}

- (void)shareItem {
    [MJLog shareLogFileWithDays:_curType];
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return self.arrData.count;
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    NSDictionary *dic = self.arrData[row];
    return dic[@"title"];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component {
    NSDictionary *dic = self.arrData[row];
    _curType = [dic[@"type"] integerValue];
}


/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
