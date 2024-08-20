# 定义删除特定行及其之前行的函数
def remove_lines(lines, target_line, num_lines_before):
    target_index = -1
    while True:
        try:
            target_index = lines.index(target_line, target_index + 1)
            # 计算要删除的起始行索引
            start_index = max(target_index - num_lines_before, 0)
            # 删除指定行及其之前的行
            del lines[start_index:target_index + 1]
        except ValueError:
            # 如果目标行不存在，则退出循环
            break

    return lines

# 定义文件路径
input_file = 'input.txt'
output_file = 'input_clean.txt'
target_line = "Zephyr Project Documentation, Release 3.5.0\n"  # 注意：这里要匹配换行符
num_lines_before = 6

# 读取文件内容
with open(input_file, 'r', encoding='utf-8') as file:
    lines = file.readlines()

# 处理文件，删除所有出现的目标行及其之前的行
cleaned_lines = remove_lines(lines, target_line, num_lines_before)

# 将修改后的内容写入新文件
with open(output_file, 'w', encoding='utf-8') as file:
    file.writelines(cleaned_lines)

print("File processing complete.")
