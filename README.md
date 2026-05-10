# Cat Chess
A fully functional 2D Chess game built in C++ using the **Raylib** graphics library. This version features custom cat-themed player profiles, standard chess rules (including special moves), and a modular, clean-code architecture.

---

## Features

### Gameplay logic
*   **Complete Move Validation:** All pieces (Pawn, Rook, Knight, Bishop, Queen, King) follow standard chess movement rules.
*   **Special Moves:** 
    *   **Castling:** Validates that the king/rook haven't moved and the path is not under attack.
    *   **En Passant:** Fully implemented pawn capture logic.
    *   **Pawn Promotion:** Interactive menu to promote pawns to a Queen, Rook, Bishop, or Knight.
*   **Advanced Game State:** Automatic detection of **Check**, **Checkmate**, and **Stalemate**.

### Visuals & UI
*   **Pre-Game Menu:** A selection screen for White and Black players to choose from **7 unique cat avatars**.
*   **Dynamic UI:**
    *   Sidebars showing the current player's turn.
    *   Visual "Check" indicator (King's square turns red).
    *   Real-time display of **Captured Pieces**.
    *   End-game popup with winner announcements.
*   **Responsive Input:** Smooth mouse-based piece selection and movement.

---

## Project Structure

The code is organized using **Object-Oriented Programming (OOP)** and a **Logic-Rendering Separation** pattern.

| File | Purpose |
| :--- | :--- |
| `main.cpp` | The entry point that initializes the game loop. |
| `render.h / .cpp` | Handles the Raylib window, texture loading, drawing functions, and mouse input. |
| `pieces.h / .cpp` | Contains the base `Piece` class and specialized logic for all 6 chess pieces. |
| `check.h / .cpp` | A dedicated math engine for calculating king safety and move simulation. |
| `menu.h / .cpp` | Manages the cat-selection state and menu navigation. |

---

## Controls

### **Menu Screen**
*   **Left/Right Arrow Keys:** Navigate through the 7 cat avatars.
*   **Enter:** Confirm selection (First for White, then for Black).

### **In-Game**
*   **Left Mouse Click:** Select a piece / Move a piece.
*   **Left Mouse Click (on same piece):** Deselect piece.
*   **ESC:** Exit the game at any time.

---

## Installation & Setup

1.  **Prerequisites:**
    *   C++ Compiler (GCC, Clang, or MSVC).
    *   [Raylib 5.0+](https://www.raylib.com/).
2.  **Asset Setup:**
    *   Ensure an `assets/` folder exists in your executable directory.
    *   Required assets: `cat1.jpeg` through `cat7.jpeg`, `white-pawn.png` (etc.), and `FreeSans.otf`.
3.  **Compilation (Example using G++):**
    ```bash
    g++ main.cpp render.cpp pieces.cpp check.cpp menu.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -o CatChess.exe
    ```

---

## License
This project was developed for educational purposes as part of a C++ Programming assignment. Feel free to use and modify! 

**Have fun playing Cat Chess!** 