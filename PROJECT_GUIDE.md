# Intensive Guide: Building C Projects

This guide provides a roadmap for building the four projects listed in your roadmap, progressing from basic syntax to advanced memory management.

---

## 1. Rule-Based Calculator / Unit Converter
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

## 4. Text-Based Adventure Game
**Focus:** Pointers & Dynamic Memory

### Key Concepts
- **Pointers:** Use pointers to link different "Room" structs together (creating a Graph structure).
- **Dynamic Allocation:** Use `malloc()` and `free()` if you want to create a world of variable size.
- **Nested Structs:** A `Room` struct should contain a description and pointers to adjacent rooms (e.g., `struct Room *north;`).

### Implementation Steps
1. **Map Design:** Sketch a simple map on paper first.
2. **Room Linking:** Initialize room pointers to connect them (e.g., `kitchen.north = &livingRoom;`).
3. **Game Loop:** 
    - Print the current room's description.
    - Ask for a direction (North, South, East, West).
    - Update the `currentRoom` pointer based on the choice.
4. **Inventory (Optional):** Use a linked list to store items the player picks up, requiring pointer manipulation to add/remove items.
