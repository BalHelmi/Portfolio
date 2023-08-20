from tkinter import *
from chat import get_response, bot_name
from PIL import Image, ImageTk

BG_GRAY = "#ABB2B9"
BG_COLOR = "#17202A"
TEXT_COLOR = "#EAECEE"

FONT = "Helvetica 14"
FONT_BOLD = "Helvetica 13 bold"

class ChatApplication:
    
    def __init__(self):
        self.window = Tk()
        self._is_full_screen = False  # Keep track of full-screen mode
        self._setup_main_window()
        
    def run(self):
        self.window.mainloop()
        
    def _setup_main_window(self):
        self.window.title("Chat")
        self.window.geometry("470x550")  
        self.window.configure(bg=BG_COLOR)
        
        # head label
        head_label = Label(self.window, bg=BG_COLOR, fg=TEXT_COLOR,
                           text="Welcome", font=FONT_BOLD, pady=10)
        head_label.pack(fill=X)
        
        # tiny divider
        line = Label(self.window, width=450, bg=BG_GRAY)
        line.pack(fill=X)
        
        # text widget
        self.text_widget = Text(self.window, bg=BG_COLOR, fg=TEXT_COLOR,
                                font=FONT, padx=5, pady=5)
        self.text_widget.pack(fill=BOTH, expand=True)
        self.text_widget.configure(cursor="arrow", state=DISABLED)
        
        # scroll bar
        scrollbar = Scrollbar(self.text_widget)
        scrollbar.pack(side=RIGHT, fill=Y)
        scrollbar.configure(command=self.text_widget.yview)
        
        # bottom label
        bottom_label = Label(self.window, bg=BG_GRAY, height=80)
        bottom_label.pack(fill=X, side=BOTTOM)
        
        # message entry box
        self.msg_entry = Entry(bottom_label, bg="#2C3E50", fg=TEXT_COLOR, font=FONT)
        self.msg_entry.pack(fill=BOTH, side=LEFT, padx=5, pady=5, expand=True)
        self.msg_entry.focus()
        self.msg_entry.bind("<Return>", self._on_enter_pressed)
        
        # send button
        send_button = Button(bottom_label, text="Send", font=FONT_BOLD, width=20, bg=BG_GRAY,
                             command=lambda: self._on_enter_pressed(None))
        send_button.pack(side=RIGHT, padx=5, pady=5)
        
        # full screen button
        full_screen_button = Button(bottom_label, text="Toggle Full Screen", font=FONT_BOLD, width=20, bg=BG_GRAY,
                                    command=self._toggle_full_screen)
        full_screen_button.pack(side=LEFT, padx=5, pady=5)
     
    def _on_enter_pressed(self, event):
        msg = self.msg_entry.get()
        self._insert_message(msg, "You")
        
    def _insert_message(self, msg, sender):
        if not msg:
            return 
        
        self.msg_entry.delete(0, END)
        msg1 = f"{sender}: {msg}\n\n"
        self.text_widget.configure(state=NORMAL)
        self.text_widget.insert(END, msg1)
        self.text_widget.configure(state=DISABLED)
        
        msg2 = f"{bot_name}: {get_response(msg)}\n\n"
        self.text_widget.configure(state=NORMAL)
        self.text_widget.insert(END, msg2)
        self.text_widget.configure(state=DISABLED)
        
        self.text_widget.see(END)
    
    def _toggle_full_screen(self):
        self._is_full_screen = not self._is_full_screen
        self.window.attributes("-fullscreen", self._is_full_screen)
        if not self._is_full_screen:
            self.window.geometry("470x550")  
        
if __name__ == "__main__":
    app = ChatApplication()
    app.run()
