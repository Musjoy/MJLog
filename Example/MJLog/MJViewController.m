//
//  MJViewController.m
//  MJLog
//
//  Created by XDD2333 on 07/03/2019.
//  Copyright (c) 2019 XDD2333. All rights reserved.
//

#import "MJViewController.h"
#import "ModuleCapability.h"
#import "Logging.h"

@interface MJViewController ()

@end

@implementation MJViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
    btn.frame = CGRectMake(40, 100, [UIScreen mainScreen].bounds.size.width - 80, 60);
    btn.backgroundColor = [UIColor blueColor];
    [btn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btn setTitle:@"输出log" forState:UIControlStateNormal];
    [btn addTarget:self action:@selector(btnAction) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btn];
    
    UILabel *lblWrite = [[UILabel alloc] initWithFrame:CGRectMake(40, CGRectGetMaxY(btn.frame) + 20, 140, 30)];
    lblWrite.font = [UIFont systemFontOfSize:20];
    lblWrite.text = @"写入文件";
    lblWrite.textColor = [UIColor blueColor];
    [self.view addSubview:lblWrite];

    UISwitch *switchWrite = [[UISwitch alloc] initWithFrame:CGRectMake(CGRectGetMaxX(lblWrite.frame) + 20, CGRectGetMinY(lblWrite.frame), 50, 30)];
    switchWrite.on = [MJLog shard].writeToFile;
    [switchWrite addTarget:self action:@selector(switchWrite:) forControlEvents:UIControlEventValueChanged];
    [self.view addSubview:switchWrite];

    
    UIButton *btnshare = [UIButton buttonWithType:UIButtonTypeCustom];
    btnshare.frame = CGRectMake(40, 40 + CGRectGetMaxY(lblWrite.frame), [UIScreen mainScreen].bounds.size.width - 80, 40);
    btnshare.backgroundColor = [UIColor blueColor];
    [btnshare setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btnshare setTitle:@"Share" forState:UIControlStateNormal];
    [btnshare addTarget:self action:@selector(shareAction) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btnshare];
    
    UIButton *btnVC = [UIButton buttonWithType:UIButtonTypeCustom];
    btnVC.frame = CGRectMake(40, 40 + CGRectGetMaxY(btnshare.frame), [UIScreen mainScreen].bounds.size.width - 80, 40);
    btnVC.backgroundColor = [UIColor blueColor];
    [btnVC setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btnVC setTitle:@"去控制界面" forState:UIControlStateNormal];
    [btnVC addTarget:self action:@selector(toVC) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:btnVC];
}

- (void)btnAction {
    LogTrace(@"123");
}

- (void)shareAction {
    [MJLog shareLogFileWithDays:1];
}

- (void)toVC {
    /// 实际代码中自行配置打开此界面逻辑，最好是难以被触发的，例如连续点击10次，长按5秒之类的
    MJLogHandleViewController *vc = [[MJLogHandleViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)switchWrite:(UISwitch *)sender {
    [MJLog shard].writeToFile = sender.isOn;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
