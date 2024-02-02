# Задаем шестнадцатеричные значения байтов
hex_values = ['a9', '11', '40']

# Преобразуем шестнадцатеричные значения в целые числа
decimal_values = [int(hex_value, 16) for hex_value in hex_values]

# Преобразуем целые числа в байтовую строку
data = bytes(decimal_values)

# Записываем данные в файл в кодировке UTF-8
with open("file2.txt", "wb") as file:
    file.write(data)
