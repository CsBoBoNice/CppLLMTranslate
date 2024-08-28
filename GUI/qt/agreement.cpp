/*
 * @Date: 2024-08-23 13:55:54
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 10:55:30
 * @FilePath: /CppLLMTranslate/GUI/qt/agreement.cpp
 */
// agreement.cpp
#include "agreement.h"
#include "cJSON.h"

agreement &agreement::getInstance()
{
    // 静态成员变量，用于存储单例对象
    static agreement instance;
    return instance;
}

void agreement::setStatus(AgreementStatus status)
{
    std::lock_guard<std::mutex> lock(mtx);
    this->status = status;
}

AgreementStatus agreement::getStatus() const
{
    std::lock_guard<std::mutex> lock(mtx);
    return status;
}

agreementInfo agreement::parseJson(const std::string &jsonStr)
{
    agreementInfo info;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    if (root == nullptr) {
        // 解析失败
        return info;
    }

    cJSON *cmd = cJSON_GetObjectItem(root, "cmd");
    if (cmd != nullptr) {
        info.cmd = cmd->valueint;
    }

    cJSON *msg = cJSON_GetObjectItem(root, "msg");
    if (msg != nullptr) {
        info.msg = msg->valuestring;
    }

    cJSON *system = cJSON_GetObjectItem(root, "system");
    if (system != nullptr) {
        info.system = system->valuestring;
    }

    cJSON *chat_prefix = cJSON_GetObjectItem(root, "chat_prefix");
    if (chat_prefix != nullptr) {
        info.chat_prefix = chat_prefix->valuestring;
    }

    cJSON *chat_suffix = cJSON_GetObjectItem(root, "chat_suffix");
    if (chat_suffix != nullptr) {
        info.chat_suffix = chat_suffix->valuestring;
    }

    cJSON *user_msg_1 = cJSON_GetObjectItem(root, "user_msg_1");
    if (user_msg_1 != nullptr) {
        info.user_msg_1 = user_msg_1->valuestring;
    }

    cJSON *user_msg_2 = cJSON_GetObjectItem(root, "user_msg_2");
    if (user_msg_2 != nullptr) {
        info.user_msg_2 = user_msg_2->valuestring;
    }

    cJSON *user_msg_3 = cJSON_GetObjectItem(root, "user_msg_3");
    if (user_msg_3 != nullptr) {

        info.user_msg_3 = user_msg_3->valuestring;
    }

    cJSON *assistant_msg_1 = cJSON_GetObjectItem(root, "assistant_msg_1");
    if (assistant_msg_1 != nullptr) {
        info.assistant_msg_1 = assistant_msg_1->valuestring;
    }

    cJSON *assistant_msg_2 = cJSON_GetObjectItem(root, "assistant_msg_2");
    if (assistant_msg_2 != nullptr) {
        info.assistant_msg_2 = assistant_msg_2->valuestring;
    }

    cJSON *assistant_msg_3 = cJSON_GetObjectItem(root, "assistant_msg_3");
    if (assistant_msg_3 != nullptr) {
        info.assistant_msg_3 = assistant_msg_3->valuestring;
    }

    cJSON_Delete(root);
    return info;
}

std::string agreement::wrapToJson(const agreementInfo &info)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "cmd", info.cmd);
    cJSON_AddStringToObject(root, "msg", info.msg.c_str());
    cJSON_AddStringToObject(root, "system", info.system.c_str());
    cJSON_AddStringToObject(root, "chat_prefix", info.chat_prefix.c_str());
    cJSON_AddStringToObject(root, "chat_suffix", info.chat_suffix.c_str());
    cJSON_AddStringToObject(root, "user_msg_1", info.user_msg_1.c_str());
    cJSON_AddStringToObject(root, "user_msg_2", info.user_msg_2.c_str());
    cJSON_AddStringToObject(root, "user_msg_3", info.user_msg_3.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_1", info.assistant_msg_1.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_2", info.assistant_msg_2.c_str());
    cJSON_AddStringToObject(root, "assistant_msg_3", info.assistant_msg_3.c_str());

    char *jsonStr = cJSON_Print(root);
    std::string result(jsonStr);
    free(jsonStr);
    cJSON_Delete(root);
    return result;
}

// 默认英译中
agreementInfo agreement::default_en_to_zh()
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

// 默认中译英
agreementInfo agreement::default_zh_to_en()
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

// 默认聊天
agreementInfo agreement::default_chat()
{
    agreementInfo info;
    info.cmd = (int)AgreementCmd::translate_msg;
    info.system = "你是一个乐于助人的助手。";
    info.chat_prefix = "\n";
    info.chat_suffix = "\n";
    info.user_msg_1 = "你好";
    info.user_msg_2 = "使用中文回答我的问题。";
    info.user_msg_3 = "回答问题尽可能简短。";

    info.assistant_msg_1 = "你好有什么可以帮助您。";
    info.assistant_msg_2 = "好的，我将使用中文回答您的问题。";
    info.assistant_msg_3 = "好的，我将使用我将尽可能简短的回答您的问题。";
    return info;
}
