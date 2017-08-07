#include "UIConnectorInfo.h"


UIConnectorInfo::UIConnectorInfo():
	host_(""),
	user_(""),
	password_(""),
	port_(0),
	window_(NULL),
	button_return_(NULL),
	button_cancel_(NULL),
	input_host_(NULL),
	input_user_(NULL),
	input_password_(NULL),
	input_port_(NULL),
	is_info_prepared_(false)
{

}

UIConnectorInfo::~UIConnectorInfo()
{

}

UIConnectorInfo& UIConnectorInfo::GetInstance()
{
	return ConnectorInfoSingleton::get_mutable_instance();
}

void UIConnectorInfo::OnInitizeDialog()
{
	window_ = new Fl_Window(50, 50, 300, 125, "Connector Login Info");
	window_->box(FL_UP_BOX);
	window_->begin();

	input_host_ = new Fl_Input(60, 5, 145, 25,  "host");
	input_user_ = new Fl_Input(60, 35, 145, 25, "user");
	input_password_ = new Fl_Input(75, 65, 220, 25, "password");
	input_port_     = new Fl_Int_Input(60, 95, 145, 25, "port");	
	button_return_  = new Fl_Button(210, 5, 85, 25, "OK");
	button_cancel_  = new Fl_Button(210, 35, 85, 25, "Cancel");

	input_host_->box(FL_UP_BOX);
	input_user_->box(FL_UP_BOX);
	input_password_->box(FL_UP_BOX);
	input_port_->box(FL_UP_BOX);
	button_return_->box(FL_UP_BOX);
	button_cancel_->box(FL_UP_BOX);

	input_host_->labelfont(FL_COURIER_BOLD);
	input_user_->labelfont(FL_COURIER_BOLD);
	input_password_->labelfont(FL_COURIER_BOLD);
	input_port_->labelfont(FL_COURIER_BOLD);
	button_return_->labelfont(FL_COURIER_BOLD);
	button_cancel_->labelfont(FL_COURIER_BOLD);

	input_host_->textfont(FL_COURIER_BOLD);
	input_user_->textfont(FL_COURIER_BOLD);
	input_password_->textfont(FL_COURIER_BOLD);
	input_port_->textfont(FL_COURIER_BOLD);

	button_return_->callback(return_callback, this);
	button_cancel_->callback(cancel_callback, this);
	button_return_->color(FL_BLACK);
	button_return_->labelcolor(FL_GREEN);
	button_cancel_->color(FL_BLACK);
	button_cancel_->labelcolor(FL_MAGENTA);	

	window_->end();
	window_->set_modal();
}

void UIConnectorInfo::show()
{
	window_->show();
}

void UIConnectorInfo::hide()
{
	host_ = input_host_->value();
	user_ = input_user_->value();
	password_ = input_password_->value();
	port_ = (unsigned int)atoi(input_port_->value());
	window_->hide();
	input_host_->value("");
	input_user_->value("");
	input_password_->value("");
	input_port_->value("");
	is_info_prepared_ = true;
}

void UIConnectorInfo::leave()
{
	window_->hide();
	is_info_prepared_ = false;
}

void UIConnectorInfo::return_callback(Fl_Widget*, void* param_)
{
	UIConnectorInfo* ui_ = (UIConnectorInfo*)param_;
	ui_->hide();
}

void UIConnectorInfo::cancel_callback(Fl_Widget*, void* param_)
{
	UIConnectorInfo* ui_ = (UIConnectorInfo*)param_;
	ui_->leave();
}