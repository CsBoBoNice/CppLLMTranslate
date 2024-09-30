/*
 * @Date: 2024-08-28 14:04:01
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 13:50:19
 * @FilePath: /CppLLMTranslate/GUI/qt/ConfigManager.cpp
 */
#include "ConfigManager.h"
#include <QApplication>
#include "cJSON.h"

std::mutex ConfigManager::mutex_;

ConfigManager &ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

std::string ConfigManager::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ""; // 文件打开失败，返回空字符串
    }

    QTextStream in(&file);
    std::string content = in.readAll().toStdString();
    file.close();
    return content;
}

bool ConfigManager::saveFile(const QString &filePath, const std::string &content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // 文件打开失败，返回 false
    }

    QTextStream out(&file);
    out << QString::fromStdString(content);
    file.close();
    return true;
}

agreementInfo ConfigManager::default_config_en_to_zh()
{
    agreementInfo info;
    info.cmd = (int)AgreementCmd::translate_msg;
    info.system = "你是专业翻译员，你需要将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。";
    info.chat_prefix = "将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。\n\n[待翻译内容开始]\n\n";
    info.chat_suffix = "\n[待翻译内容结束]\n\n开始将英文文档翻译成简体中文。\n\n";
    info.user_msg_1 = "Clipboard_Singleton_thread";
    info.user_msg_2 = "getInstance";
    info.user_msg_3 =
        "Life is actually like the weather, with its sunny days, cloudy days, and occasional rain showers. It's "
        "the natural order of things. Life isn't simple, but we should strive to simplify it as much as  possible.";
    info.assistant_msg_1 = "剪贴板单例线程";
    info.assistant_msg_2 = "获得实例";
    info.assistant_msg_3 = "生活其实和天气一样，有晴，有阴，偶尔还会下点雨，自然规律，生活不简单尽量简单过。";

    return info;
}

agreementInfo ConfigManager::Get_config_en_to_zh()
{
    if (!en_to_zh_change) {
        return en_to_zh_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_config_en_to_zh();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/en_to_zh_config.json";
    QFile file(info_path);
    if (file.exists() == true) {
        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_config_en_to_zh();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    en_to_zh_info = info; // 保存到全局变量
    return info;
}

void ConfigManager::Set_config_en_to_zh(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/en_to_zh_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    en_to_zh_change = true; // 设置改变标志
    en_to_zh_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_config_zh_to_en()
{
    agreementInfo info;

    info.cmd = (int)AgreementCmd::translate_msg;
    info.system = "你是专业翻译员，你需要将简体中文翻译成英文,翻译后仅输出翻译内容，无需其他解释说明。";
    info.chat_prefix = "将简体中文翻译成英文,翻译后仅输出翻译内容，无需其他解释说明。\n\n[待翻译内容开始]\n\n";
    info.chat_suffix = "\n[待翻译内容结束]\n\n开始将将简体中文翻译成英文。\n\n";
    info.user_msg_1 = "精美";
    info.user_msg_2 = "生活其实和天气一样，有晴，有阴，偶尔还会下点雨，自然规律，生活不简单尽量简单过。";
    info.user_msg_3 = "帮助";

    info.assistant_msg_1 =
        "exquisite\n精致的，精美的；\nelegant\n（人）高雅的，举止优雅的；\nfineness\n美好；纯度；细微；优雅；";
    info.assistant_msg_2 =
        "Life is actually like the weather, with its sunny days, cloudy days, and occasional rain showers. It's "
        "the natural order of things. Life isn't simple, but we should strive to simplify it as much as  possible.";
    info.assistant_msg_3 =
        "help\n帮助，援助；\nassist\n帮助，协助；\nassistance\n帮助，援助；\naid\n帮助；援助，救助；";

    return info;
}

agreementInfo ConfigManager::Get_config_zh_to_en()
{
    if (!zh_to_en_change) {
        return zh_to_en_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_config_zh_to_en();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/zh_to_en_config.json";
    QFile file(info_path);
    if (file.exists() == true) {
        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_config_zh_to_en();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    zh_to_en_info = info;    // 保存到全局变量
    zh_to_en_change = false; // 设置改变标志
    return info;
}

void ConfigManager::Set_config_zh_to_en(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/zh_to_en_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    zh_to_en_change = true; // 设置改变标志
    zh_to_en_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_config_chat()
{
    agreementInfo info;
    info.cmd = (int)AgreementCmd::translate_msg;
    info.system = "你是一个乐于助人的助手。";
    info.chat_prefix.clear();
    info.chat_suffix.clear();
    info.user_msg_1.clear();
    info.user_msg_2.clear();
    info.user_msg_3.clear();

    info.assistant_msg_1.clear();
    info.assistant_msg_2.clear();
    info.assistant_msg_3.clear();
    return info;
}

agreementInfo ConfigManager::Get_config_chat()
{
    if (!chat_change) {
        return chat_info; // 未改变，直接返回全局变量
    }
    agreementInfo info = default_config_chat();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/chat_config.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_config_chat();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    chat_info = info;    // 保存到全局变量
    chat_change = false; // 设置改变标志
    return info;
}

void ConfigManager::Set_config_chat(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/chat_config.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    chat_change = true; // 设置改变标志
    chat_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_get_prompt_md_file()
{
    agreementInfo info;
    info.system = R"(
- Role: 专业翻译员
- Background: 用户需要将英文文档翻译成简体中文，并且需要保持文档的原始格式和专业术语的准确性。
- Profile: 你是一位经验丰富的翻译专家，精通英文和简体中文，擅长技术文档的翻译工作。
- Skills: 语言翻译能力、文档格式理解、专业术语准确性把控。
- Goals: 确保文档翻译后，格式、结构、链接和图片标记保持原样，同时专业术语翻译准确无误。
- Constrains: 翻译过程中，不得更改文档的任何非文本内容，包括链接、图片标记等。
- OutputFormat: 翻译后的文档以Markdown格式输出。翻译后仅输出翻译内容，无需其他解释说明。
- Workflow:
  1. 阅读并理解原文本内容及其格式。
  2. 翻译文本内容，同时保留所有格式化指令。
  3. 检查翻译后的文档，确保链接和图片标记未被更改，专业术语准确。
- Initialization: 欢迎来到文档翻译服务，我将帮助您将英文文档翻译成简体中文，同时保留所有格式化指令和文档结构。请发送您需要翻译的文档。
)";
    info.chat_prefix = R"(
[待翻译内容开始]
)";
    info.chat_suffix = R"(
[待翻译内容结束]

开始翻译任务，翻译后仅输出翻译内容，无需其他解释说明。

[优质翻译开始]
)";
    info.user_msg_1 = R"(
## Memory requirements
To decode and display a GIF animation the following amount of RAM is required:
- `LV_COLOR_DEPTH 8`: 3 x image width x image height
- `LV_COLOR_DEPTH 16`: 4 x image width x image height
- `LV_COLOR_DEPTH 32`: 5 x image width x image height

## Example
```eval_rst
.. include:: ../../examples/libs/gif/index.rst
```

## API

```eval_rst
.. doxygenfile:: lv_gif.h
  :project: lvgl
```
)";
    info.user_msg_2.clear();
    info.user_msg_3.clear();

    info.assistant_msg_1 = R"(
## 内存要求

为了解码和显示GIF动画，需要以下数量的RAM：
- `LV_COLOR_DEPTH 8`: 3 x 图像宽度 x 图像高度
- `LV_COLOR_DEPTH 16`: 4 x 图像宽度 x 图像高度
- `LV_COLOR_DEPTH 32`: 5 x 图像宽度 x 图像高度

## 示例

```eval_rst
.. include:: ../../examples/libs/gif/index.rst
```

## API

```eval_rst
.. doxygenfile:: lv_gif.h
  :project: lvgl
```
)";
    info.assistant_msg_2.clear();
    info.assistant_msg_3.clear();
    return info;
}

agreementInfo ConfigManager::get_prompt_md_file()
{
    if (!md_file_change) {
        return md_file_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_get_prompt_md_file();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_md.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_get_prompt_md_file();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    md_file_info = info;    // 保存到全局变量
    md_file_change = false; // 设置改变标志
    return info;
}

void ConfigManager::set_prompt_md_file(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_md.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    md_file_change = true; // 设置改变标志
    md_file_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_get_prompt_txt_file()
{
    agreementInfo info;
    info.system = R"(
- Role: 专业翻译员
- Background: 用户需要将英文文档翻译成简体中文，并且需要保持文档的原始格式和专业术语的准确性。
- Profile: 你是一位经验丰富的翻译专家，精通英文和简体中文，擅长技术文档的翻译工作。
- Skills: 语言翻译能力、文档格式理解、专业术语准确性把控。
- Goals: 确保文档翻译后，格式、结构、链接和图片标记保持原样，同时专业术语翻译准确无误。
- Constrains: 翻译过程中，不得更改文档的任何非文本内容，包括链接、图片标记等。
- OutputFormat: 翻译后的文档保持格式。翻译后仅输出翻译内容，无需其他解释说明。
- Workflow:
  1. 阅读并理解原文本内容及其格式。
  2. 翻译文本内容，同时保留所有格式化指令。
  3. 检查翻译后的文档，确保链接和图片标记未被更改，专业术语准确。
- Initialization: 欢迎来到文档翻译服务，我将帮助您将英文文档翻译成简体中文，同时保留文档结构。请发送您需要翻译的文档。
)";
    info.chat_prefix = R"(
[待翻译内容开始]
)";
    info.chat_suffix = R"(
[待翻译内容结束]

开始翻译任务，翻译后仅输出翻译内容，无需其他解释说明。

[优质翻译开始]
)";
    info.user_msg_1 = R"(
GIF decoder
Allow using GIF images in LVGL. Based on https://github.com/lecram/gifdec

When enabled in `lv_conf.h` with `LV_USE_GIF` `lv_gif_create(parent)` can be used to create a gif widget.

`lv_gif_set_src(obj, src)` works very similarly to `lv_img_set_src`. As source, it also accepts images as variables (`lv_img_dsc_t`) or files.


Convert GIF files to C array
To convert a GIF file to byte values array use [LVGL's online converter](https://lvgl.io/tools/imageconverter). Select "Raw" color format and "C array" Output format.


Use GIF images from file
For example:
```c
lv_gif_set_src(obj, "S:path/to/example.gif");
```

Note that, a file system driver needs to be registered to open images from files. Read more about it [here](https://docs.lvgl.io/master/overview/file-system.html) or just enable one in `lv_conf.h` with `LV_USE_FS_...`

)";
    info.user_msg_2.clear();
    info.user_msg_3.clear();

    info.assistant_msg_1 = R"(
GIF解码器

允许在LVGL中使用GIF图像。基于 https://github.com/lecram/gifdec

在`lv_conf.h`中启用`LV_USE_GIF`后，可以使用`lv_gif_create(parent)`来创建一个GIF控件。

`lv_gif_set_src(obj, src)`与`lv_img_set_src`非常相似。它也接受变量（`lv_img_dsc_t`）或文件作为图像源。

将GIF文件转换为C数组

要将GIF文件转换为字节数组，请使用[LVGL在线转换器](https://lvgl.io/tools/imageconverter)。选择“Raw”颜色格式和“C array”输出格式。

从文件中使用GIF图像

例如：
```c
lv_gif_set_src(obj, "S:path/to/example.gif");
```

请注意，需要注册文件系统驱动程序才能从文件中打开图像。更多信息请阅读[这里](https://docs.lvgl.io/master/overview/file-system.html)，或者在`lv_conf.h`中启用`LV_USE_FS_...`

)";
    info.assistant_msg_2.clear();
    info.assistant_msg_3.clear();
    return info;
}

agreementInfo ConfigManager::get_prompt_txt_file()
{
    if (!txt_file_change) {
        return txt_file_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_get_prompt_txt_file();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_txt.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_get_prompt_txt_file();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    txt_file_info = info;    // 保存到全局变量
    txt_file_change = false; // 设置改变标志
    return info;
}

void ConfigManager::set_prompt_txt_file(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_txt.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    txt_file_change = true; // 设置改变标志
    txt_file_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_get_prompt_rst_file()
{
    agreementInfo info;
    info.system = R"(
- Role: RST文档翻译专家
- Background: 用户需要将reStructuredText（RST）格式的英文文档翻译成简体中文，同时必须保留文档的所有RST语法标记和结构，段落中的行内标记前后添加必要的空格。
- Profile: 你是一位专业的翻译人员，专注于技术文档的翻译，并且对RST语法有深入的了解。
- Skills: 精通中英文翻译，熟悉RST语法规则，能够准确保留文档的格式和结构。
- Goals: 提供一个高质量的翻译，确保翻译后的文档在结构、布局和格式上与原文档完全对应，同时满足RST语法段落中的行内标记前后添加必要的空格。
- Constrains: 翻译过程中必须保留所有RST语法标记，包括但不限于标题、列表、代码块等。链接和图片标记不得更改，超链接标记需与文字描述完全一致，并在前后添加适当的空格以符合RST语法。
- OutputFormat: 保留RST格式。翻译后仅输出翻译内容，无需其他解释说明。
- Workflow:
  1. 仔细阅读并分析原始RST文档，理解其结构和格式。
  2. 文档内容翻译成中文，确保语义准确，表达自然。
  3. 校对翻译内容，确保所有RST语法标记和结构得到正确保留，RST语法段落中的行内标记前后添加必要的空格。
- Examples:
  - 原文: "*Emphasis*" 应翻译为 " *强调* "，注意星号前后的空格。
  - 原文: "**Emphasis**" 应翻译为 " **强调** "，注意双星号前后的空格。
  - 原文: "``inline text``" 应翻译为 " ``内联文字`` "，注意反引号前后的空格。
  - 原文: "`hyperlink`_" 应翻译为 " `超链接`_ "，注意前后的空格。
- Initialization: 准备好开始翻译任务，确保翻译后的文档在结构和布局上与原文本完全一致。
)";
    info.chat_prefix = R"(
[待翻译内容开始]
)";
    info.chat_suffix = R"(
[待翻译内容结束]

开始翻译任务，保留RST格式，确保翻译后的文档在结构和布局上与原文本完全一致。翻译后仅输出翻译内容，无需其他解释说明。

[优质翻译开始]
)";
    info.user_msg_1 = R"(
.. _dt-from-c:

Devicetree access from C/C++
############################

This guide describes Zephyr's ``<zephyr/devicetree.h>`` API for reading the
devicetree from C source files. It assumes you're familiar with the concepts in
:ref:`devicetree-intro` and :ref:`dt-bindings`. See :ref:`dt-reference` for
reference material.

A note for Linux developers
***************************

Linux developers familiar with devicetree should be warned that the API
described here differs significantly from how devicetree is used on Linux.

.. _dt-node-identifiers:
)";
    info.user_msg_2 = R"(
Node identifiers
****************

To get information about a particular devicetree node, you need a *node
identifier* for it. This is a just a C macro that refers to the node.

These are the main ways to get a node identifier:

By node label
   Use :c:func:`DT_NODELABEL()` to get a node identifier from a :ref:`node
   label <dt-node-labels>`. Node labels are often provided by SoC :file:`.dtsi`
   files to give nodes names that match the SoC datasheet, like ``i2c1``,
   ``spi2``, etc.

By alias
   Use :c:func:`DT_ALIAS()` to get a node identifier for a property of the
   special ``/aliases`` node. This is sometimes done by applications (like
   :zephyr:code-sample:`blinky`, which uses the ``led0`` alias) that need to
   refer to *some* device of a particular type ("the board's user LED") but
   don't care which one is used.

Two node identifiers which refer to the same node are identical and can be used
interchangeably.
)";
    info.user_msg_3 = R"(
.. _dt-node-main-ex:

Here's a DTS fragment for some imaginary hardware we'll return to throughout
this file for examples:

.. literalinclude:: main-example.dts
   :language: devicetree
   :start-after: start-after-here

Here are a few ways to get node identifiers for the ``i2c@40002000`` node:

- ``DT_PATH(soc, i2c_40002000)``
- ``DT_NODELABEL(i2c1)``
- ``DT_ALIAS(sensor_controller)``
- ``DT_INST(x, vnd_soc_i2c)`` for some unknown number ``x``. See the
  :c:func:`DT_INST()` documentation for details.

.. important::

   Non-alphanumeric characters like dash (``-``) and the at sign (``@``) in
   devicetree names are converted to underscores (``_``). The names in a DTS
   are also converted to lowercase.
)";

    info.assistant_msg_1 = R"(
.. _dt-from-c:

从C/C++访问设备树
##############################

本指南描述了 Zephyr 的 ``<zephyr/devicetree.h>`` API，用于从 C 源文件中读取设备树。它假设您熟悉 :ref:`devicetree-intro` 和 :ref:`dt-bindings` 中的概念。有关参考资料，请参阅 :ref:`dt-reference` 。


Linux开发者注意事项
**********************************

熟悉设备树的Linux开发者应该注意，这里描述的API与Linux中设备树的使用方法存在显著差异。

.. _dt-node-identifiers:
)";
    info.assistant_msg_2 = R"(
节点标识符
*********************

要获取特定设备树节点信息，您需要一个 *节点标识符* 。这只是一个指向该节点的C宏。

以下是获取节点标识符的主要方法：

通过节点标签
   使用 :c:func:`DT_NODELABEL()` 从 :ref:`节点标签 <dt-node-labels>` 获取节点标识符。节点标签通常由SoC :file:`.dtsi` 文件提供，以便节点名称与SoC数据表匹配，如 ``i2c1`` 、 ``spi2`` 等等。

通过别名
   使用 :c:func:`DT_ALIAS()` 获得特殊 ``/aliases`` 节点的属性节点标识符。这通常由应用程序（如 :zephyr:code-sample:`blinky` ，它使用 ``led0`` 别名）完成，这些应用程序需要引用特定类型的 *某些* 设备（例如“板上的用户LED”）但不在乎使用哪个。

两个指向同一节点的节点标识符是相同的，可以互换使用。
)";
    info.assistant_msg_3 = R"(
.. _dt-node-main-ex:

以下是一个我们将在此文件中多次引用的假设硬件的DTS片段：

.. literalinclude:: main-example.dts
   :language: devicetree
   :start-after: start-after-here

以下是获取 ``i2c@40002000`` 节点节点标识符的几种方法：

- ``DT_PATH(soc, i2c_40002000)``
- ``DT_NODELABEL(i2c1)``
- ``DT_ALIAS(sensor_controller)``
- ``DT_INST(x, vnd_soc_i2c)`` 对于某些未知的数字 ``x`` 。有关详细信息，请参阅 :c:func:`DT_INST()` 文档。

.. important::

   设备树名称中的非字母数字字符，如连字符 ( ``-`` ) 和at符号 ( ``@`` ) ，将转换为下划线 ( ``_`` ) 。DTS中的名称也转换为小写。
)";
    return info;
}

agreementInfo ConfigManager::get_prompt_rst_file()
{
    if (!rst_file_change) {
        return rst_file_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_get_prompt_rst_file();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_rst.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_get_prompt_rst_file();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    rst_file_info = info;    // 保存到全局变量
    rst_file_change = false; // 设置改变标志
    return info;
}

void ConfigManager::set_prompt_rst_file(const agreementInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_rst.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    rst_file_change = true; // 设置改变标志
    rst_file_info = Info;   // 保存到全局变量
}

agreementInfo ConfigManager::default_get_prompt_h_file()
{
    agreementInfo info;
    info.system = R"(
- Role: 编程语言翻译专家
- Background: 用户需要将C语言文件中的注释从英文翻译成简体中文，同时保留所有代码格式和注释中的关键标识符。
- Profile: 你是一位精通C语言和中英文翻译的专业翻译员，能够准确翻译注释而不改变代码的任何功能。
- Skills: 精通C语言、中英文互译、代码格式保留。
- Goals: 确保注释内容被翻译成简体中文，同时保留所有代码格式和关键标识符。
- Constrains: 翻译必须准确无误，不得改变代码的任何逻辑结构和关键标识符。
- OutputFormat: 翻译后的C语言文件，注释部分为简体中文，代码和关键标识符保持原样。翻译后仅输出翻译内容，无需其他解释说明。
- Workflow:
  1. 识别C语言文件中的注释和关键标识符。
  2. 翻译注释内容，同时确保关键标识符如@defgroup, @ingroup, @addtogroup, @cond等保持原样。
  3. 将翻译后的注释和未改变的代码格式整合，生成最终的文件。
- Initialization: 欢迎使用C语言注释翻译服务。请上传您的C语言文件，我们将仅翻译注释内容，同时保留所有代码格式和关键标识符。
)";
    info.chat_prefix = R"(
[待翻译内容开始]
)";
    info.chat_suffix = R"(
[待翻译内容结束]

开始翻译任务，翻译后仅输出翻译内容，无需其他解释说明。

[优质翻译开始]
)";
    info.user_msg_1 = R"(
/*
 * Copyright (c) 2014-2015 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Variables needed for system clock
 *
 *
 * Declare variables used by both system timer device driver and kernel
 * components that use timer functionality.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <util.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup clock_apis
 * @{
 */

/**
 * @brief Compare timeouts for equality
 *
 * The k_timeout_t object is an opaque struct that should not be
 * inspected by application code.  This macro exists so that users can
 * test timeout objects for equality with known constants
 * (e.g. K_NO_WAIT and K_FOREVER) when implementing their own APIs in
 * terms of Zephyr timeout constants.
 *
 * @return True if the timeout objects are identical
 */
#define K_TIMEOUT_EQ(a, b) ((a).ticks == (b).ticks)

/** number of nanoseconds per micorsecond */
#define NSEC_PER_USEC 1000U

/** @} */

/** @cond INTERNAL_HIDDEN */

/* Converts between absolute timeout expiration values (packed into
 * the negative space below K_TICKS_FOREVER) and (non-negative) delta
 * timeout values.
 */
#define Z_TICK_ABS(t) (K_TICKS_FOREVER - 1 - (t))

/** @endcond */

/* kernel clocks */

/*
 * We default to using 64-bit intermediates in timescale conversions,
 * but if the HW timer cycles/sec, ticks/sec and ms/sec are all known
 * to be nicely related, then we can cheat with 32 bits instead.
 */
/**
 * @addtogroup clock_apis
 * @{
 */

#ifdef CLOCK_EXISTS

/**
 * @brief Remaining time to given timepoint
 *
 * Returns the timeout interval between current time and provided timepoint.
 * If the timepoint is now in the past or if it was created with `K_NO_WAIT`
 * then `K_NO_WAIT` is returned. If it was created with `K_FOREVER` then
 * `K_FOREVER` is returned.
 *
 * @param timepoint Timepoint for which a timeout value is wanted.
 * @retval Corresponding timeout value.
 *
 * @see sys_timepoint_calc()
 */
k_timeout_t sys_timepoint_timeout(k_timepoint_t timepoint);

/**
 * @brief Compare two timepoint values.
 *
 * This function is used to compare two timepoint values.
 *
 * @param a Timepoint to compare
 * @param b Timepoint to compare against.
 * @return zero if both timepoints are the same. Negative value if timepoint @a a is before
 * timepoint @a b, positive otherwise.
 */
static inline int sys_timepoint_cmp(k_timepoint_t a, k_timepoint_t b)
{
	if (a.tick == b.tick) {
		return 0;
	}
	return a.tick < b.tick ? -1 : 1;
}

#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
)";
    info.user_msg_2.clear();
    info.user_msg_3.clear();

    info.assistant_msg_1 = R"(
/*
 * 版权所有 (c) 2014-2015 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief 系统时钟所需的变量
 *
 *
 * 声明由系统定时器设备驱动程序和内核组件使用的变量，这些组件使用定时器功能。
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include <util.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup clock_apis
 * @{
 */

/**
 * @brief 比较超时是否相等
 *
 * k_timeout_t对象是一个不透明结构，应用程序代码不应检查。此宏存在是为了让用户在实现自己的API时，可以测试超时对象与已知常量（例如 K_NO_WAIT 和 K_FOREVER）是否相等。
 *
 * @return 如果超时对象相同，则返回真
 */
#define K_TIMEOUT_EQ(a, b) ((a).ticks == (b).ticks)

/** 每微秒的纳秒数 */
#define NSEC_PER_USEC 1000U

/** @} */

/** @cond INTERNAL_HIDDEN */

/*
 * 将绝对超时到期值（打包在K_TICKS_FOREVER下方的负空间中）与（非负）增量超时值之间进行转换。
 */
#define Z_TICK_ABS(t) (K_TICKS_FOREVER - 1 - (t))

/** @endcond */

/* 内核时钟 */

/*
 * 我们默认使用64位中间变量进行时间尺度转换，
 * 但是如果硬件定时器周期/秒，滴答/秒和毫秒/秒都已知是良好相关的，那么我们可以用32位作弊。
 */
/**
 * @addtogroup clock_apis
 * @{
 */

#ifdef CLOCK_EXISTS

/**
 * @brief 给定时间点剩余时间
 *
 * 返回当前时间与提供时间点之间的超时间隔。
 * 如果时间点现在是过去的，或者它是由 `K_NO_WAIT` 创建的，那么返回 `K_NO_WAIT`。
 * 如果它是由 `K_FOREVER` 创建的，那么返回 `K_FOREVER`。
 *
 * @param timepoint 想要超时值的时间点。
 * @retval 对应的超时值。
 *
 * @see sys_timepoint_calc()
 */
k_timeout_t sys_timepoint_timeout(k_timepoint_t timepoint);

/**
 * @brief 比较两个时间点值。
 *
 * 此函数用于比较两个时间点值。

 * @param a 要比较的时间点
 * @param b 要比较的时间点
 * @return 如果两个时间点相同，则返回零。如果时间点 @a a 在时间点 @a b 之前，则返回负值，否则返回正值。
 */
static inline int sys_timepoint_cmp(k_timepoint_t a, k_timepoint_t b)
{
	if (a.tick == b.tick) {
		return 0;
	}
	return a.tick < b.tick ? -1 : 1;
}

#endif

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */
)";
    info.assistant_msg_2.clear();
    info.assistant_msg_3.clear();
    return info;
}

agreementInfo ConfigManager::get_prompt_h_file()
{
    if (!h_file_change) {
        return h_file_info; // 未改变，直接返回全局变量
    }

    agreementInfo info = default_get_prompt_h_file();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_h.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = agreement::getInstance().parseJson(info_json);
        if (info.system.empty()) {
            info = default_get_prompt_h_file();
            saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, agreement::getInstance().wrapToJson(info)); // 配置异常恢复默认
    }
    h_file_info = info;    // 保存到全局变量
    h_file_change = false; // 设置改变标志
    return info;
}

void ConfigManager::set_prompt_h_file(const agreementInfo &Info)
{

    QString info_path = QCoreApplication::applicationDirPath() + "/prompot_h.json";
    saveFile(info_path, agreement::getInstance().wrapToJson(Info));
    h_file_change = true; // 设置改变标志
    h_file_info = Info;   // 保存到全局变量
}

TranslationSetInfo TranslationSetInfoParseJson(const std::string &jsonStr)
{
    TranslationSetInfo info;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    if (root == nullptr) {
        return info;
    }

    cJSON *paragraph_effective = cJSON_GetObjectItem(root, "paragraph_effective");
    if (paragraph_effective != nullptr) {
        info.paragraph_effective = paragraph_effective->valueint;
    }

    cJSON *paragraph_min = cJSON_GetObjectItem(root, "paragraph_min");
    if (paragraph_min != nullptr) {
        info.paragraph_min = paragraph_min->valueint;
    }

    cJSON *paragraph_max = cJSON_GetObjectItem(root, "paragraph_max");
    if (paragraph_max != nullptr) {
        info.paragraph_max = paragraph_max->valueint;
    }

    cJSON *Input_file_path = cJSON_GetObjectItem(root, "Input_file_path");
    if (Input_file_path != nullptr) {
        info.Input_file_path = Input_file_path->valuestring;
    }

    cJSON *Output_file_path = cJSON_GetObjectItem(root, "Output_file_path");
    if (Output_file_path != nullptr) {
        info.Output_file_path = Output_file_path->valuestring;
    }

    cJSON *Cut_file_path = cJSON_GetObjectItem(root, "Cut_file_path");
    if (Cut_file_path != nullptr) {
        info.Cut_file_path = Cut_file_path->valuestring;
    }

    cJSON *Reference_file_path = cJSON_GetObjectItem(root, "Reference_file_path");
    if (Reference_file_path != nullptr) {
        info.Reference_file_path = Reference_file_path->valuestring;
    }

    cJSON *Success_file_path = cJSON_GetObjectItem(root, "Success_file_path");
    if (Success_file_path != nullptr) {
        info.Success_file_path = Success_file_path->valuestring;
    }

    cJSON_Delete(root);
    return info;
}

std::string TranslationSetInfoWrapToJson(const TranslationSetInfo &info)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "paragraph_effective", info.paragraph_effective);
    cJSON_AddNumberToObject(root, "paragraph_min", info.paragraph_min);
    cJSON_AddNumberToObject(root, "paragraph_max", info.paragraph_max);
    cJSON_AddStringToObject(root, "Input_file_path", info.Input_file_path.c_str());
    cJSON_AddStringToObject(root, "Output_file_path", info.Output_file_path.c_str());
    cJSON_AddStringToObject(root, "Cut_file_path", info.Cut_file_path.c_str());
    cJSON_AddStringToObject(root, "Reference_file_path", info.Reference_file_path.c_str());
    cJSON_AddStringToObject(root, "Success_file_path", info.Success_file_path.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);
    return result;
}

TranslationSetInfo ConfigManager::default_get_TranslationSetInfo()
{
    QString InputPath = QCoreApplication::applicationDirPath() + "/input";
    QString OutputPath = QCoreApplication::applicationDirPath() + "/output";
    QString CutPath = QCoreApplication::applicationDirPath() + "/output/cut";
    QString ReferencePath = QCoreApplication::applicationDirPath() + "/output/reference";
    QString SuccessPath = QCoreApplication::applicationDirPath() + "/output/success";

    TranslationSetInfo info;
    info.paragraph_effective = 512;
    info.paragraph_min = 3072;
    info.paragraph_max = 4096;
    info.Input_file_path = InputPath.toStdString();
    info.Output_file_path = OutputPath.toStdString();
    info.Cut_file_path = CutPath.toStdString();
    info.Reference_file_path = ReferencePath.toStdString();
    info.Success_file_path = SuccessPath.toStdString();
    return info;
}

TranslationSetInfo ConfigManager::get_TranslationSetInfo()
{
    if (!TranslationSetInfo_change) {
        return TranslationSetInfo_info; // 未改变，直接返回全局变量
    }

    TranslationSetInfo info = default_get_TranslationSetInfo();
    std::string info_json;
    QString info_path = QCoreApplication::applicationDirPath() + "/TranslationSetInfo.json";
    QFile file(info_path);
    if (file.exists() == true) {

        info_json = readFile(info_path);
        info = TranslationSetInfoParseJson(info_json);
        if (info.Input_file_path.empty()) {
            info = default_get_TranslationSetInfo();
            saveFile(info_path, TranslationSetInfoWrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, TranslationSetInfoWrapToJson(info)); // 配置异常恢复默认
    }

    TranslationSetInfo_info = info;    // 保存到全局变量
    TranslationSetInfo_change = false; // 清除改变标志
    return info;
}

void ConfigManager::set_TranslationSetInfo(const TranslationSetInfo &Info)
{
    QString info_path = QCoreApplication::applicationDirPath() + "/TranslationSetInfo.json";
    saveFile(info_path, TranslationSetInfoWrapToJson(Info));
    TranslationSetInfo_change = true; // 设置改变标志
    TranslationSetInfo_info = Info;   // 保存到全局变量
}

TranslationProgressConfig TranslationProgressParseJson(const std::string &jsonStr)
{
    TranslationProgressConfig info;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    if (root == nullptr) {
        return info;
    }

    cJSON *file_index = cJSON_GetObjectItem(root, "file_index");
    if (file_index != nullptr) {
        info.file_index = file_index->valueint;
    }

    cJSON *paragraph_index = cJSON_GetObjectItem(root, "paragraph_index");
    if (paragraph_index != nullptr) {
        info.paragraph_index = paragraph_index->valueint;
    }

    cJSON_Delete(root);
    return info;
}

std::string TranslationProgressWrapToJson(const TranslationProgressConfig &info)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "file_index", info.file_index);
    cJSON_AddNumberToObject(root, "paragraph_index", info.paragraph_index);

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);
    return result;
}

TranslationProgressConfig ConfigManager::default_get_TranslationProgressConfig()
{
    TranslationProgressConfig info;
    info.file_index = 0;
    info.paragraph_index = 0;
    return info;
}

TranslationProgressConfig ConfigManager::get_TranslationProgressConfig(std::string Input_file_path)
{
    if (!TranslationProgressConfig_change) {
        return TranslationProgressConfig_info; // 未改变，直接返回全局变量
    }

    TranslationProgressConfig info = default_get_TranslationProgressConfig();
    QString info_path = Input_file_path.c_str();
    QFile file(info_path);
    if (file.exists() == true) {
        std::string info_json = readFile(info_path);
        info = TranslationProgressParseJson(info_json); // 解析json
        if (info.file_index == 0) {
            info = default_get_TranslationProgressConfig();
            saveFile(info_path, TranslationProgressWrapToJson(info)); // 配置异常恢复默认
        }
    } else {
        saveFile(info_path, TranslationProgressWrapToJson(info)); // 配置异常恢复默认
    }

    TranslationProgressConfig_info = info; // 保存到全局变量
    TranslationProgressConfig_change = false;
    return info;
}

void ConfigManager::set_TranslationProgressConfig(const TranslationProgressConfig &Info, std::string Input_file_path)
{
    QString info_path = Input_file_path.c_str();
    info_path += "/TranslationProgressConfig.json";
    saveFile(info_path, TranslationProgressWrapToJson(Info));
    TranslationProgressConfig_change = true;
    TranslationProgressConfig_info = Info; // 保存到全局变量
}
