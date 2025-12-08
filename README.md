# Card_Game_Simulator
A command line interface for card games as part of a Y2 project.

Some style decisions:
- If a comment begins with "note:", it means its commenting on how c works, not how the code works. basically its more for me than someone else as I will use this code to relearn c when I inevitably forget it in future.
- If I give a function a pointer to an array, use arr[] not arr* for clarity, even though it makes no difference.
- If pass a function a variable (by value or by ref), I try to change the name slightly so I can clearly see which variables are local to that function. Just helps when learning.
- /* */ comments are to segment files and describe functions.
- Use a double space before case to indent after switch, then double space to indent case code after. 

More standard style decisions:
- Use 4 spaces for indentation (just how my vs code is set up).
- use func() [one space] {\n indented_code \n}.