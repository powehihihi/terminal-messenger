#include "DialoguesWindow.hpp"
#include <curses.h>
#include <ncurses.h>
#include <string>

DialoguePreview::DialoguePreview(ChatId id, const int h, const int w, int x, bool isTop, bool isBot):
 id(id), h(h), w(w), win(newwin(h, w, x, 0)), isTop(isTop), isBot(isBot), chosen(isTop) {
   if (chosen)
     Choose();
   else
     Unchoose();
   UpdateText();
}
/*
DialoguePreview& DialoguePreview::operator=(const DialoguePreview& d) {
  id = d.id;
  return *this;
}
*/
void DialoguePreview::Unchoose() {
  chosen = false;
  int tl=ACS_LTEE, tr=ACS_RTEE, bl=ACS_LTEE, br=ACS_RTEE;
  if (isTop) {
    tl = ACS_ULCORNER;
    tr = ACS_URCORNER;
  }
  /*
  if (isBot) {
    bl = ACS_LLCORNER;
    br = ACS_LRCORNER;
  }
  */
  wborder(win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, tl, tr, bl, br);
  wrefresh(win);
}
void DialoguePreview::Choose() {
  chosen = true;
  int tl=ACS_LTEE, tr=ACS_RTEE, bl=ACS_LTEE, br=ACS_RTEE;
  if (isTop) {
    tl = ACS_ULCORNER;
    tr = ACS_URCORNER;
  }
  if (isBot) {
    bl = ACS_LLCORNER;
    br = ACS_LRCORNER;
  }
  wborder(win, '>', '<', 'v', '^', tl, tr, bl, br);
  //wborder(win, '|', '|', '-', '-', tl, tr, bl, br);
  wrefresh(win);
}

void DialoguePreview::WriteText(std::string&& txt) {
  if (txt.size() > w-2) {
    txt.erase(w-3-3, std::string::npos).append("...");
  }
  mvwprintw(win, 1, 1, "%s", txt.c_str());
  wrefresh(win);
}

void DialoguePreview::SetId(ChatId cid) {
  id = cid;
  UpdateText();
}
ChatId DialoguePreview::GetId() {
  return id;
}
void DialoguePreview::UpdateText() {
  WriteText(std::string(w-2, ' '));
  WriteText(std::to_string(static_cast<int>(id)));
}


DialoguesWindow::DialoguesWindow(const std::vector<ChatId>& Chats): Chats(Chats), 
                                            height(LINES-2),
                                            width(COLS/3), 
                                            MainWindow(create_newwin(height, width, 0, 0))
{
  if (Chats.empty())
    return;
  int x=0;
  const int previews_amount = std::min((height-1)/(previews_height-1), static_cast<int>(Chats.size()));
  for (size_t i=0; i<previews_amount; ++i, x+=previews_height-1) {
    bool isTop=false, isBot=false;
    if (i==0)
      isTop = true;
    if (i+1==previews_amount)
      isBot=true;
    Previews.emplace_back(Chats[i], previews_height, width, x, isTop, isBot);
  }
  Previews[0].Choose();
}

void DialoguesWindow::Down() {
  if (chosenPreview+topChatId+1==Chats.size())
    return;
  if (chosenPreview+1<Previews.size()) {
    Previews[chosenPreview].Unchoose();
    Previews[++chosenPreview].Choose();
    return;
  }
  ++topChatId;
  for (size_t i=0; i<Previews.size(); ++i) {
    Previews[i].SetId(Chats[topChatId+i]);
    Previews[i].UpdateText();
  }
}
void DialoguesWindow::Up() {
  if (topChatId+chosenPreview==0)
    return;
  if (chosenPreview>0) {
    Previews[chosenPreview].Unchoose();
    Previews[--chosenPreview].Choose();
    return;
  }
  --topChatId;
  for (size_t i=0; i<Previews.size(); ++i) {
    Previews[i].SetId(Chats[topChatId+i]);
    Previews[i].UpdateText();
  }
}

ChatId DialoguesWindow::Enter() {
  return //Previews[chosenPreview].GetId();
         Chats[topChatId+chosenPreview];
}

void DialoguesWindow::NewChat(ChatId new_id) {
  Chats.push_back(new_id);
  if (Chats.size()>height/previews_height)
    return;
  int x = (previews_height-1)*Previews.size();
  Previews.emplace_back(Chats.back(), previews_height, width, x, false, true);
}


// Old
/*
DialoguesWindow::DialoguesWindow(const std::vector<ChatId>& Chats): Chats(Chats), 
                                            height(LINES-2),
                                            width(COLS/3), 
                                            MainWindow(create_newwin(height, width, 0, 0))
  {
    if (Windows.empty())
      return;
    int x=0;
    for (size_t i=0; i<Windows.size(); ++i) {
      Windows[i] = newwin(dialogue_height, width, x, 0);
      wborder(Windows[i], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LTEE, ACS_RTEE);
      wrefresh(Windows[i]);
      x += dialogue_height-1;
    }
    wborder(Windows[0], '|', '|', '-', '-', ACS_ULCORNER, ACS_URCORNER, ACS_LTEE, ACS_RTEE);
    wrefresh(Windows[0]);
    chosen = 0;
    Choose(2);
  }


void DialoguesWindow::Choose(int i) {
  if (i>=Windows.size() || i==chosen)
    return;
  int i_tl, c_tl, i_tr, c_tr; 
  i_tl = c_tl = ACS_LTEE;
  i_tr = c_tr = ACS_RTEE;
  if (i==0) {
    i_tl = ACS_ULCORNER;
    i_tr = ACS_URCORNER;
  }
  if (chosen==0) {
    c_tr = ACS_URCORNER;
    c_tl = ACS_ULCORNER;
  }
  wborder(Windows[chosen], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, c_tl, c_tr, ACS_LTEE, ACS_RTEE);
  wrefresh(Windows[chosen]);
  wborder(Windows[i], '|', '|', '-', '-', i_tl, i_tr, ACS_LTEE, ACS_RTEE);
  wrefresh(Windows[i]);
  chosen = i;
}
void DialoguesWindow::Up() {
  if (chosen==0)
    return;
  Choose(chosen-1);
}
void DialoguesWindow::Down() {
  if (chosen!=Windows.size()-1)
    Choose(chosen+1);
}
ChatId DialoguesWindow::Enter() {
  // TODO
  return ChatId(0);
}

void DialoguesWindow::NewChat(ChatId id) {
  Chats.push_back(id);
  if (Windows.size() < height/dialogue_height) {
    int i = Windows.size();
    Windows.push_back(newwin(dialogue_height, width, i*(dialogue_height-1), 0));
    wborder(Windows[i], ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_LTEE, ACS_RTEE, ACS_LTEE, ACS_RTEE);
    wrefresh(Windows[i]);
  }
}
*/
