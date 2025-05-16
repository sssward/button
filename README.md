# button

## 简介
简单高效的高内聚低耦合的基于状态机的按键扫描框架，可以实现按键阈值可配置的单击、双击、长按的判定

## 使用方法
1.注册按键
```c
Button button1;
```
2.编写获取按键状态的回调函数
```c
uint8_t getButtonVal(void){}
```
3.初始化按键对象
```c
buttonInit(&button1,IDLE_LEVEL_LOW,20,100,getButtonVal);
```

4.根据需求编写对应事件的回调函数
```c
void singleClickHandler(void){}
void doubleClickHandler(void){}
void longPressHandler(void){}
```
5.注册按键的对应事件
```c
buttonLink(&button1,SINGLE_CLICK,singleClickHandler);
buttonLink(&button1,DOUBLE_CLICK,doubleClickHandler);
buttonLink(&button1,LONG_PRESS,longPressHandler);
```
6.固定10ms的周期执行按键扫描函数
```c
buttonScan(&button1);
```
