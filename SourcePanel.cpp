#include "SourcePanel.h"

SourcePanel::SourcePanel(wxFrame* MainFrame) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	//the layout (from top to bottom)
	std::vector<wxString> DummyChoices = { "ahcjbhknklml", "b" }; //changed when filesystem is implemented
	SelectBody = new wxChoiceVector(this, wxID_ANY, DummyChoices);;
	CreateNew = new wxButton(this, 10001, "Create New Beam", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	YValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	ZValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	//continue vertical
	//now aligned horizontally
	xDirectionValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	yDirectionValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	zDirectionValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//continue vertical
	wxButton* RefreshViewer = new wxButton(this, wxID_ANY, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));

	//add them to sizers
	TranslationSizer->Add(x, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(XValue, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(y, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(YValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(z, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(ZValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

	RotationSizer->Add(xDirection, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(xDirectionValue, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(yDirection, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(yDirectionValue, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(zDirection, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(zDirectionValue, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);

	VerticalLayout->Add(PickExisting, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SelectBody, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(CreateNew, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetPosition, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(TranslationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetRotation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RotationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RefreshViewer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	this->SetSizer(VerticalLayout);

}