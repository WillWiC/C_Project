# Building C Projects
---
## 0. C Tutorial
**Focus:** Basic Tutorials to familar the syntax and key concepts of C

---
## 1. Rule-Based Calculator
**Focus:** Basics (Variables, I/O, Control Flow)

### Key Concepts
- **Data Types:** Use `float` or `double` for precision in conversions.
- **Input/Output:** `printf()` for display and `scanf()` for user input.
- **Control Flow:** `switch` statements for menu selection; `if-else` for logic.

### Implementation Steps
1. **Define the Menu:** Use a loop (like `while(1)`) to keep the program running until the user chooses to exit.
2. **Handle Categories:** Create functions for each conversion type (e.g., `convertTemperature()`, `convertWeight()`) to keep `main()` clean.
3. **Logic:** Implement standard formulas (e.g., `F = (C * 9/5) + 32`).
4. **Validation:** Check if the user enters valid menu numbers.

---

## 2. Command-Line To-Do List
**Focus:** Arrays & Strings

### Key Concepts
- **Arrays:** Use a 2D char array `char tasks[MAX_TASKS][MAX_LENGTH]` to store strings.
- **String Handling:** Include `<string.h>` for `strcpy()`, `strlen()`, and `strcmp()`.
- **Loops:** Iterate through arrays to display or find specific tasks.

### Implementation Steps
1. **State Management:** Use a counter variable to track how many tasks are currently in the list.
2. **Add Task:** Get a string from the user (use `fgets()` instead of `scanf()` to allow spaces) and store it in the next available array index.
3. **View Tasks:** Loop through the array from `0` to `counter` and print each string with its index.
4. **Delete/Complete:** Shift array elements to "remove" an item, or use a parallel boolean array to mark tasks as "done".

---

## 3. Simple Database (Phonebook)
**Focus:** Structs & File I/O

### Key Concepts
- **Structs:** Define a `Contact` struct to group `name`, `phone`, and `email`.
- **File Pointers:** Use `FILE *fp` with `fopen()`, `fclose()`, `fprintf()`, and `fscanf()`.
- **Modes:** Use `"a"` for appending new contacts and `"r"` for reading the database.

### Implementation Steps
1. **Define the Schema:** Create a `struct` that represents one entry in your database.
2. **Persistent Storage:** Every time a contact is added, write it directly to a `.txt` or `.dat` file.
3. **Search Logic:** To find a contact, open the file in read mode, loop through every entry, and use `strcmp()` to find a match.
4. **Formatting:** Use structured delimiters (like commas or tabs) to make the data easy to parse later.

---