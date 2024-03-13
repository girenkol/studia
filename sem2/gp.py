def count_characters(file_path):
    try:
        with open(file_path, 'r') as file:
            content = file.read()
            char_count = {}
            for char in content:
                if char.isalpha():
                    char = char.lower()
                    if char in char_count:
                        char_count[char] += 1
                    else:
                        char_count[char] = 1
            return char_count
    except FileNotFoundError:
        print("Nie można odnaleźć pliku:", file_path)

file_path = input("Podaj ścieżkę do pliku tekstowego: ")
character_counts = count_characters(file_path)
if character_counts:
    print("Ilość poszczególnych liter w pliku:")
    for char in sorted(character_counts.keys()):
        print(f"Ilość '{char}': {character_counts[char]}")