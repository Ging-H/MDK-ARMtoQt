# MDK-ARMtoQt
将keil MDK-ARM项目工程转换程Qt5的项目工程,并且使用ARMCC编译源码  
需要将Keil MDK-ARM v5转换成Qt5.9.0的项目工程,使用使用ARMcc编译,如果可以,将会使用Openocd+DAPLink调试  
# TODO List  
- [x] 编写一个软件将MDK-ARM项目工程转换成Qt5工程  
  - [x] 从*.uvprojx文件里面读取工程文件信息,包括源码文件,头文件,全局宏定义,以及编译选项  
  - [x] 将软件作为Keil MDK-ARM的自定义工具,添加到Keil MDK-ARM中,使用Keil MDK-ARM进行调用  
  - [ ] 将源工程的信息保存到新工程里面  
- [x] 在Qt5.9.0里面添加新的编译器(ARMCC),添加相关的头文件路径,从而避免使用Qt编辑源码的时候有一些错误的警告  
- [ ] 编写一个软件根据项目工程/配置生成makefile(或者与Keil MSK-ARM生成的*.bat相似的)文件  
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

