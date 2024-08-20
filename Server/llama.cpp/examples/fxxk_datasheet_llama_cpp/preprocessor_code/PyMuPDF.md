解决的问题
pdf左右分栏的文本难以提取 作者：穿云蟒 https://www.bilibili.com/read/cv16356882/ 出处：bilibili



创建虚拟环境并同时安装 python3.11：

```
conda create --name pdf_to_txt_2 python=3.11
```

进入环境

```
conda activate pdf_to_txt_2
```

安装

```
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple/ PyMuPDF
pip install -i https://pypi.tuna.tsinghua.edu.cn/simple/ tqdm
pip install  -i https://pypi.tuna.tsinghua.edu.cn/simple/ beautifulsoup4 
```

处理思路
先将pdf转为html，再利用html的标签树层级关系，对信息内容进行提取，要优于将pdf文件按块划分后用ocr图相识别文字的方式进行提取。 

```
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
                with open("分析文献文本提取.txt", 'a', encoding='utf-8') as text_file:
                    text_file.write(text)
                    text_file.write('\n')


#主函数
input_path = r'example.pdf'
html_path = r'example.html'
pdf2html(input_path,html_path )  #pdf转html
html2txt(html_path)  #解析html保存为txt 
```

碰到的问题:

```
新款的PyMuPDF包里 getText方法更名为了 get_text

转换出来的代码中&my_sem变成了'&amp;'

from bs4 import BeautifulSoup
from html import unescape

# ... 其他代码 ...

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
                print(text)
                with open("分析文献文本提取.txt", 'a', encoding='utf-8') as text_file:
                    text_file.write(text)
                    text_file.write('\n')

# ... 其他代码 ...

导入了unescape函数，并在将res[0]添加到text变量之前，使用unescape来解码HTML实体。这样，&amp;会被转换回&，以及其他任何HTML实体都会被转换回它们的原始字符形式。

```





