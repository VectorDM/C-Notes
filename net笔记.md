#.net笔记
##TextBlock等文本类控件 
枚举类型，是否可见
TextBoxId.Visibility = Visibility.Collapsed;//可见
Visible//不可见
isEnabled//不可用
Background//背景颜色
foreground//文本颜色
FontSize//字体大小
##text
Text//内容
IsReadOnly=“False”//只读
TextWrapping = "Wrap"//可换行
MaxLength = “5”//最多输入5字符
##PasswordBox密码框
Password=“”//输入的密码属性
##可空类型
可空（Null）数据类型：引用类型。自定义类，string，大部分类
不可空：值类型。int ；boolean；dicimal；datetime等
##CheckBox
chB.IsChecked
bool?类型
if(chB.IsChecked==ture){}或者
if((bool) chB.IsChecked){}均可
##RadioButton
//多个只能有一个被选中
//多组时，每一个取一个组名GroupName
##dataPicker//.net4.0
//dataPicker.SelectedDate;
//dataPicker.SelectedDate=DateTime.Today;
var value = DatePicker.SelectedDate;
MessageBox.Show(value == null ? "请选择日期" : value.ToString());
##DateTime类
 DateTime.Today;//日期今天0时0分0秒
 DateTime.Now;//日期时间现在
##Image控件
 source = “路径”
##ProgressBar进度条
minimum=“0”//最小值
maximum=“100”//最大值
value=“60”//当前值
IsIndeterminate="true"//不确定模式
##
 



