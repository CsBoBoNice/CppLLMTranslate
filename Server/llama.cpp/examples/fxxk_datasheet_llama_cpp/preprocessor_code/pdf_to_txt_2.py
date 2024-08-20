import os
import fitz
from tqdm import tqdm
from bs4 import BeautifulSoup
from html import unescape
import re

#将pdf转成html
def pdf2html(input_path,html_path):
    doc = fitz.open(input_path)
    print(doc)
    html_content =''
    for page in tqdm(doc):
        html_content += page.get_text('html')
    print('开始输出html文件')
    html_content +="</body></html>"
    with open(html_path, 'w', encoding = 'utf-8', newline='')as fp:
        fp.write(html_content)

# 使用Beautifulsoup解析本地html
def html2txt(html_path):
    with open(html_path, 'r', encoding='utf-8') as html_file:
        htmlhandle = html_file.read()
        soup = BeautifulSoup(htmlhandle, "html.parser")
        for div in soup.find_all('div'):
            for p in div:
                text = ''
                for span in p:
                    p_info = '<span .*?>(.*?)</span>'  # 提取规则
                    res = re.findall(p_info, str(span))  # findall函数
                    if len(res) == 0:
                        pass
                    else:
                        # 将列表中的字符串内容合并加到行字符串中，并解码HTML实体
                        text += unescape(res[0])
                # print(text)
                with open("input.txt", 'a', encoding='utf-8') as text_file:
                    text_file.write(text)
                    text_file.write('\n')


#主函数
input_path = r'input.pdf'
html_path = r'input.html'
pdf2html(input_path,html_path )  #pdf转html
html2txt(html_path)  #解析html保存为txt


print('html to txt ok!')

os.system('exit')

 