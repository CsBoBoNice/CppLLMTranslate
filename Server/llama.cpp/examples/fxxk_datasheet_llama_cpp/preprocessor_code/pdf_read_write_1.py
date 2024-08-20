import fitz  # PyMuPDF

# 假设的翻译函数，接收文本并返回翻译后的文本
def translate(text):
    # 这里替换为实际的翻译函数
    return "translated " + text  # 假设的翻译结果

# 读取PDF文件
pdf_path = "example.pdf"
pdf_document = fitz.open(pdf_path)
output_pdf_path = "translated_" + pdf_path

# 创建一个新的PDF来保存翻译后的内容
output_document = fitz.open()

# 遍历每一页
for page_number in range(len(pdf_document)):
    page = pdf_document[page_number]
    
    # 创建新的PDF页面
    new_page = output_document.new_page(width=page.rect.width, height=page.rect.height)

    # 获取页面上的文本及其位置信息
    text_instances = page.get_text("dict")

    # 存储文本的位置和大小信息
    text_blocks = []

    # 获取文本位置和内容
    for block in text_instances["blocks"]:
        if block["type"] == 0:  # 确保是文本块
            text = ""
            for line in block["lines"]:
                for span in line["spans"]:
                    text += span["text"] + " "
            text_blocks.append((text.strip(), block["bbox"]))

    # 对文本进行翻译
    translated_text_blocks = [(translate(text), bbox) for text, bbox in text_blocks]

    # 将翻译后的文本写回PDF
    for text, bbox in translated_text_blocks:
        # 插入文本的位置是矩形的左下角
        insert_point = fitz.Point(bbox[0], bbox[1])
        # 插入翻译后的文本
        new_page.insert_text(insert_point, text)

    # 复制原始页面的内容（包括图片和格式）
    contents = page.get_contents()
    for content in contents:
        new_page.insert_pdf_page(content)

# 保存修改后的PDF
output_document.save(output_pdf_path)

# 关闭PDF文档
pdf_document.close()
output_document.close()
