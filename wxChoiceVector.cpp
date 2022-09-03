#include "wxChoiceVector.h"

wxChoiceVector::wxChoiceVector(wxWindow* parent, wxWindowID id, std::vector<wxString> choices) : wxChoice(parent,
    id)
{
    for (int i = 0; i < choices.size(); i++)
    {
        this->Append(choices.at(i));
    }
}