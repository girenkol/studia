class Turing:
    def __init__(self, alphabet, states, initial_state, accepting_state, transitions):
        self.alphabet = alphabet
        self.states = states
        self.initial_state = initial_state
        self.accepting_state = accepting_state
        self.transitions = transitions
        self.tape = []
        self.head_position = 0
        self.current_state = initial_state

    def load_definition_from_file(self, filename):
        with open(filename, 'r') as file:
            lines = file.readlines()
            self.alphabet = lines[1].split()
            self.states = lines[3].split()
            self.initial_state = lines[5].strip()
            self.accepting_state = lines[7].strip()
            self.transitions = {}
            for line in lines[9:]:
                parts = line.split()
                if len(parts) == 5:
                    current_state, current_symbol, new_state, new_symbol, direction = parts
                    if current_state not in self.transitions:
                        self.transitions[current_state] = {}
                    self.transitions[current_state][current_symbol] = (new_state, new_symbol, direction)

    def step(self):
        current_symbol = self.tape[self.head_position] if self.head_position < len(self.tape) else '#'
        if self.current_state in self.transitions and current_symbol in self.transitions[self.current_state]:
            new_state, new_symbol, direction = self.transitions[self.current_state][current_symbol]
            self.tape[self.head_position] = new_symbol
            if direction == 'R':
                self.head_position += 1
            elif direction == 'L':
                self.head_position -= 1
            self.current_state = new_state
        else:
            self.current_state = 'odrzucony'

    def run(self, input_string):
        self.tape = list(input_string) + ['#'] * 100  # Initialize tape with input string followed by blanks
        self.head_position = 0
        self.current_state = self.initial_state
        while self.current_state != self.accepting_state and self.current_state != 'odrzucony':
            self.step()
        return self.current_state == self.accepting_state

def sprawdz_pesel_z_maszyna_turinga(pesel, plik_z_definicja):
    if len(pesel) != 11 or not pesel.isdigit():
        return False
    turing_machine = Turing([], [], '', '', {})
    turing_machine.load_definition_from_file(plik_z_definicja)
    while not turing_machine.current_state == 'odrzucony':
        turing_machine.step()
    return turing_machine.current_state != 'odrzucony'

# Przykład użycia
pesel = "68013189176"
plik_definicji = "maszyna_turinga_pesel.txt"

if sprawdz_pesel_z_maszyna_turinga(pesel, plik_definicji):
    print("Numer PESEL jest poprawny.")
else:
    print("Numer PESEL jest niepoprawny.")
