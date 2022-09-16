#include "GeometryPanel.h"

GeometryPanel::GeometryPanel(wxFrame* MainFrame) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	//the layout (from top to bottom)
	std::vector<wxString> DummyChoices = {"ahcjbhknklml", "b"};
	SelectBody = new wxChoiceVector(this, wxID_ANY, DummyChoices);;
	CreateNew = new wxButton(this, 10001, "Create New Body", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	YValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	ZValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	//continue vertical
	//now aligned horizontally
	Euler1Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	Euler2Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	Euler3Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//continue vertical
	wxButton* RefreshViewer = new wxButton(this, wxID_ANY, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));

	//add them to sizers
	TranslationSizer->Add(x, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(XValue, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(y, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(YValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(z, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(ZValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

	RotationSizer->Add(Euler1, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler1Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);

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