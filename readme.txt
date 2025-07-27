编译方法一：makefile
  1.编译时，在windows命令行下，运行build_open_app.bat nwy_test_data即可编译SDK目录下的nwy_test_data的DEMO APP例子，提示成功后，会在SDK目录下生成release目录。
  2.SDK目录下的nwy_test_cli是一个包含所用功能的测试用例APP，客户可以在开发时参考，也可以烧录到模组中，通过USB的虚拟串口，连接到PC的串口工具进行功能验证。
  3.客户可以在SDK目录下自行创建代码工程目录，编译时，只需要build_open_app.bat加上目录名称即可，release的目录的文件名称是一致的。
  4.烧录APP固件时，使用SWDownloader工具进行烧录升级，打开时选择加载编译好的release目录下的nwy_open_app_dload.elf即可，然后点击start后，需要通过PC串口工具发送AT$MYDOWNLOAD=1进入下载模式后，自动升级，升级成功后，工具会提示升级成功，一般需要3秒左右。
  5.release目录下的nwy_open_app_fota.pkt，是用于客户APP本地升级的，客户可参考nwy_fota.h中的接口，用于开发客户APP的FOTA升级业务。

编译方法二：cmake
  1.编译时，在windows命令行下，运行build_open_app_cmake.bat 即可编译SDK目录下的nwy_test_demo的DEMO APP。编译完成后，会在SDK的out/hex目录下生成elf文件。
  2.SDK目录下的nwy_test_cli是一个包含所用功能的测试用例APP，客户可以在开发时参考，也可以烧录到模组中，通过USB的虚拟串口，连接到PC的串口工具进行功能验证。
  3.客户可以在SDK目录下自行创建代码工程目录，编译时，只需要在SDK根目录的CMakeLists.txt加上app名称，以及对应的源码文件即可。
  4.out/release目录下的nwy_open_app_fota.pkt，是用于客户APP本地升级的，客户可参考nwy_fota.h中的接口，用于开发客户APP的FOTA升级业务。