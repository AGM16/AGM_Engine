#ifndef _PANEL_WINDOWS_H_
#define _PANEL_WINDOWS_H_

class Panel_Windows
{
public:

	Panel_Windows();
	virtual ~Panel_Windows();

	void Visualizate_Window(bool on);
	bool Is_Active();
  
private:

	bool active_window;

};


#endif // !_PANEL_MANAGER_H_

