## 使用方法

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

   ## 打开QT工程

新生成的文件有以下4个文件

![image-20211004160811283](https://i.loli.net/2021/10/04/9DaX2WKZkANhsRb.png)

选择`*.creator`,双击打开,或者直接打开`QtCreator`,然后在`QtCreator`里面打开这个文件,或者在打开`QtCreator`之后,将这个文件拖拽到`QtCreator`上.
