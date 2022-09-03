#pragma once
#include "wx/wx.h"
#include <vector>
//I created a wrapper class for wxChoice to abstract default arguments out
//also includes a for-loop constructor for setting items from a predefined vector/array

//POSSIBLE MEMORY LEAK: check if parent wxWindow also deletes objects derived from wx objects, as well as wx objects

class wxChoiceVector : public wxChoice
{
public:
    wxChoiceVector(wxWindow* parent, wxWindowID id, std::vector<wxString> choices);
};

