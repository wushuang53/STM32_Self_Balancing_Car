# STM32_Self_Balancing_Car
基于STM32F103C8T6的两轮自平衡小车，使用PID算法实现直立平衡
##项目简介
 
本项目是一个基于 STM32 微控制器的两轮自平衡小车。通过采集姿态传感器数据，结合 PID 控制算法，驱动电机实现小车的直立平衡、速度控制和转向功能。
 
 
 
##硬件平台
 
- 主控芯片：STM32F103C8T6（ARM Cortex-M3 内核，72MHz）
- 姿态传感器：MPU6050（六轴加速度计和陀螺仪）
- 电机驱动：TB6612FNG 或 L298N 双路直流电机驱动
- 电源：7.4V 锂电池（两节 18650）
- 编码器：带增量式编码器的直流减速电机
 
 
 
##软件架构
 
plaintext
  
.
├── balance_car/           # 项目核心工程目录
│   ├── user/              # 用户应用代码目录，包含main.c和外设驱动
│   │   ├── main.c         # 程序入口，主循环和状态机
│   │   ├── app_bat.c/h    # 电池电压采集模块（ADC）
│   │   ├── app_button.c/h # 按键输入处理模块
│   │   ├── app_encoder.c/h # 编码器测速模块
│   │   ├── app_pwm.c/h    # PWM输出控制模块（电机调速）
│   │   ├── app_uart2.c/h  # 串口通信模块（调试/上位机）
│   │   ├── system_stm32f10x.c/h # 系统时钟和初始化
│   │   └── stm32f10x_it.c/h # 中断服务函数
│   ├── test/              # 模块测试代码目录
│   │   ├── bat_test.c/h   # 电池电压采集测试
│   │   ├── encoder_test.c/h # 编码器测速测试
│   │   └── pwm_test.c/h   # PWM输出测试
│   ├── my_lib/            # 自定义硬件驱动库
│   │   ├── oled.c/h       # OLED显示屏驱动
│   │   ├── spi.c/h        # SPI通信驱动
│   │   ├── i2c.c/h        # I2C通信驱动（用于MPU6050）
│   │   ├── uart.c/h       # 通用串口驱动
│   │   └── pid.c/h        # PID算法实现
│   ├── startup/           # STM32启动文件和链接脚本
│   │   └── startup_stm32f10x_md.s # 中容量产品启动文件
│   ├── std_periph_driver/ # STM32标准外设库
│   │   ├── inc/           # 外设库头文件
│   │   └── src/           # 外设库源文件
│   └── balance_car.uvprojx # Keil MDK 工程文件
├── .gitignore             # Git忽略配置，排除编译产物
├── LICENSE                # MIT 开源许可证
└── README.md              # 项目说明文档
 
 
 
 
##核心功能
 
1. 直立平衡控制：通过 MPU6050 采集姿态数据，使用串级 PID 算法控制车身倾角。
2. 速度闭环控制：结合编码器数据，实现小车速度的精确控制。
3. 转向控制：通过差速驱动实现小车转向。
4. 电池电压监测：实时监测电池电压，低电压报警。
5. 串口调试：通过串口输出姿态、速度等关键数据，便于调试和上位机监控。
 
 
 
##环境要求
 
- 开发环境：Keil MDK-ARM V5
- 调试器：ST-Link V2
- 硬件：STM32F103C8T6 核心板、MPU6050 模块、电机驱动板、电机和电池
 
##编译与下载
 
1. 克隆项目到本地：
bash
  
git clone https://github.com/wushuang53/STM32_Self_Balancing_Car.git
 
2. 打开 Keil MDK，加载  balance_car/balance_car.uvprojx  工程文件。
3. 点击  Build  按钮编译工程。
4. 连接 ST-Link 调试器，点击  Download  按钮将程序下载到开发板。
 
 
 
#关键模块说明
 
##PID 控制
 
- 直立环：控制小车保持直立，输入为倾角，输出为电机期望速度。
- 速度环：控制小车速度，输入为速度误差，输出为直立环期望倾角。
- 转向环：控制小车转向，输入为角速度误差，输出为左右电机差速。
 
##MPU6050 驱动
 
- 使用 I2C 通信读取加速度计和陀螺仪数据。
- 通过 DMP（数字运动处理器）解算姿态角，或使用卡尔曼滤波/互补滤波进行数据融合。
 
 
 
##许可证
 
本项目采用 MIT 许可证，详见 LICENSE 文件。
 

欢迎提交 Issue 和 Pull Request 来改进本项目。
 
 
 
联系方式
 
如有问题或建议，可通过 GitHub Issues 联系我。
