# 注意事项
1. 这个修改版只是将部分多余文件去掉，改用在gcc环境下使用cmake构建。

2. 此bootloader固件需刷到0x8000000，于常规应用一直

3. app固件需修改入口地址为0x8005000, 并且修改工程中system_stm32f1xx.c文件

```
//原system_stm32f1xx.c:113
//#define VECT_TAB_OFFSET  0x00000000U  
#define VECT_TAB_OFFSET  0x00005000U  //修改为左侧所示
```
