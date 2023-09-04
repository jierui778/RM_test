# View

工程采用MPU6050作为传感器，芯片为STM32F103ZET6

## task1

<font size="5">陀螺仪原始数据显示:</font>
![error](task1/photo/gyro.png)
数据拨动较小

<font size="5">加速度计原始数据显示:</font>

<font size="5">x轴垂直</font>
![error](task1/photo/AX.png)
<font size="5">y轴垂直</font>
![error](task1/photo/AY.png)
<font size="5">z轴垂直</font>
![error](task1/photo/AZ.png)

## task2

![error](task2/task2.png)
<font size="5">roll轴旋转180度结果，积分数据误差较大</font>

<font size="5">数据经过简单的均值滤波和陀螺仪去漂移处理，零点漂移仍然对积分结果有影响</font>

## task3

<font size="5">初始值</font>
![error](task3/photo/初始值.png)

<font size="5">pitch角</font>
![error](task3/photo/pitch.png)

<font size="5">roll角</font>
![error](task3/photo/pitch.png)

<font size="5">由于yaw角无磁力计校准，故该角数据无效</font>


## <font size="5">task4&5见工程文件<font>