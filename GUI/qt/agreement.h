/*
 * @Date: 2024-08-23 13:55:38
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-24 09:52:17
 * @FilePath: /lvgl/main/src/agreement.h
 */
// agreement.h
#ifndef AGREEMENT_H
#define AGREEMENT_H

#include <string>
#include <mutex>
#include "cJSON.h"

enum class AgreementStatus { idle, busy, unknown };

enum class AgreementCmd {
    success_msg,   // 翻译成功
    course_msg,    // 过程信息
    translate_msg, // 待翻译信息
    test,          // 测试连接
    unknown
};

struct agreementInfo {
    int cmd;
    std::string msg;
    std::string system;
    std::string chat_prefix;
    std::string chat_suffix;
    std::string user_msg_1;
    std::string user_msg_2;
    std::string user_msg_3;
    std::string assistant_msg_1;
    std::string assistant_msg_2;
    std::string assistant_msg_3;
};

class agreement {
  public:
    // 获取单例对象的静态方法
    static agreement &getInstance();

    // 设置和获取状态的方法，线程安全
    void setStatus(AgreementStatus status);
    AgreementStatus getStatus() const;

    // 解析json数据
    agreementInfo parseJson(const std::string &jsonStr);

    // 封装为json数据
    std::string wrapToJson(const agreementInfo &info);

  private:
    // 构造函数和析构函数私有化，防止外部创建和删除对象
    agreement() {}
    ~agreement() {}

    // 状态变量
    mutable std::mutex mtx;
    AgreementStatus status;
};

#endif // AGREEMENT_H
