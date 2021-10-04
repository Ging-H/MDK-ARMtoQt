# MDK-ARMtoQt
将keil MDK-ARM项目工程转换程Qt5的项目工程,并且使用ARMCC编译源码  
需要将Keil MDK-ARM v5转换成Qt5.9.0的项目工程,使用使用ARMcc编译,如果可以,将会使用Openocd+DAPLink调试  
# TODO List  
- [x] 编写一个软件将MDK-ARM项目工程转换成Qt5工程  
  - [x] 从*.uvprojx文件里面读取工程文件信息,包括源码文件,头文件,全局宏定义,以及编译选项  
  - [x] 将软件作为Keil MDK-ARM的自定义工具,添加到Keil MDK-ARM中,使用Keil MDK-ARM进行调用  
  - [ ] 将源工程的信息保存到新工程里面  
- [x] 在Qt5.9.0里面添加新的编译器(ARMCC),添加相关的头文件路径,从而避免使用Qt编辑源码的时候有一些错误的警告  
- [ ] 编写一个软件根据项目工程/配置生成makefile(或者与Keil MDK-ARM生成的*.bat相似的)文件  
- [ ] 编写一个软件根据makefile文件调用armcc编译,链接,生成镜像(\*.bin)文件  
# 目前进度说明  
目前以完成从Keil MDK-ARM到Qt项目工程的转换,但是由于有部分内容需要与后续的进度配合,所以仍留下部分操作未完成

## MDK-ARM v5 to Qt5.9.0
相关的源码和可执行文件在MDK5toQt文件夹中
1. 直接生成

   * 把Keil MDK(v5)工程文件(`*.uvprojx`)拖动到MDK5toQt.exe上
   * 会在`*.uvprojx`的上一级目录下生成Qt的工程文件
   * 使用`QtCreator`打开新生成的`*.creator`文件

2. 作为Keil的自定义工具生成

   * 将软件添加进keil的自定义工具菜单中  
     ![image-20211004160239091](https://i.loli.net/2021/10/04/gBycOvaWikIZPER.png)
   * 将添加一个菜单项,`Command`选择该软件的路径,`Arguments`填入`#P`  
     ![image-20211004160415859](https://i.loli.net/2021/10/04/rFzw29ivEJVnSBm.png)
   * 打开当前项目,点击`MDK5to Qt`生成Qt的项目文件  
     ![image-20211004160547960](https://i.loli.net/2021/10/04/6ctquN1DaxVX2ez.png)
   * 使用`QtCreator`打开新生成的`*.creator`文件

3. 打开QT工程

新生成的文件有以下4个文件

![image-20211004160811283](https://i.loli.net/2021/10/04/9DaX2WKZkANhsRb.png)

选择`*.creator`,双击打开,或者直接打开`QtCreator`,然后在`QtCreator`里面打开这个文件,或者在打开`QtCreator`之后,将这个文件拖拽到`QtCreator`上.
## 添加ARMCC编译套件
打开QtCreator,然后打开`工具->选项->构建和运行->编译器`,在右边添加新的编译器
1. 新的编译器选择`自定义->C`,也可以继续添加C++的编译器,不过使用单片机的程序大多都是C语言开发,选择C就足够了  
3. 在下方的具体设置那里填入编译器名称(由于头文件路径是按芯片型号分类的,所以建议带上芯片型号作区分),编译器路径,make路径(待后续完成的make软件之后可以在这里修改make路径),还需要填入`Header paths`,`ABI`选项不重要,可以不选;    
![image-20211004164256043](https://i.loli.net/2021/10/04/wfRxcuHdkNgO5Pl.png)  
3. 在最下方的错误解析器那里,选择自定义,并且点击右边的按钮编译解析器的模式,在模式栏填充错误提示的规则表达式,用于提取关键信息;可以在下面的Test里面粘贴上从Keil复制过来的一条错误提示,用于测试;规则表达式最多可以有9对括号,也就是9个子表达式,在捕获位置(Capture Position)分别选择文件名,行数,信息对应的子表达式,例如下图就是在规则表达式里面就是有三个子表达式,其中第一个子表达式就是匹配的文件名,所以`File name`选择1,第二个子表达式匹配行数,所以`Line number`选择2,第三个子表达式匹配提示信息,所以`Message`选择3,这里的规则表达式不能匹配多行数据(暂不知道原因,可能是我没找到方法),所以在信息一栏无法像keil一样准确定位到列;在`warning`选项卡也是一样的操作,用于提取警告信息,不过需要注意应该根据实际的警告提示信息来编写规则表达式(就是要注意将error改成warning,别就复制粘贴什么都不改);  
![image-20211004164846426](https://i.loli.net/2021/10/04/Pty98wSvdqCGkA7.png)
4. 注意匹配出来的文件名不要有双引号,否则QtCreator无法识别匹配结果,将会导致在编译出现错误的时候,无法双击跳转到具体的位置
5. 切换到`构建套件(kit)`选项卡,手动添加新的编译套件;名称可以随便填写,但是建议带有可以明确提示的名称;设备类型不重要,可以任意选择,如果选择Bare Metal Device(需要先添加Bare Metal插件,可以在帮助菜单->关于插件里面找到),将会在有单片机的图标;编译器选择刚新建的ARMCC编译器,c++可以选择`<No compiler>`;`Environment`选择编译器的路径;调试器可以不选,后续会使用oenocd或者stlink;Qt版本选择`<无>`;其他的可以不管;  
![image-20211004171349992](https://i.loli.net/2021/10/04/lEGR8JdZkDbPMeY.png)
6. 打开*.crator工程,在项目里面,选择自定义的编译器  
![image-20211004172756784](https://i.loli.net/2021/10/04/okg6X9P8YWjQMUV.png)  
通过选择不同的编译器对比,可以看到不同的编译器对标准库头文件的搜索路径是不一样的  
![image-20211004173107488](https://i.loli.net/2021/10/04/3ZEKfv4jUQApst1.png)
  
  



















