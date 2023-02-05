#include "NewDirPicker.h"

NewDirPicker::NewDirPicker() : wxFrame(this, wxID_ANY, "Create New Project", wxDefaultPosition, wxDefaultSize)
{
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);

	wxDirPickerCtrl* m_dirPicker1 = new wxDirPickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE);
	bSizer1->Add(m_dirPicker1, 0, wxALL, 5);

	this->SetSizer(bSizer1);
}

