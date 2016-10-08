#ifndef _PANEL_APPLICATION_H_
#define _PANEL_APPLICATION_H_



class Panel_Application
{
	public:

		Panel_Application();
		~Panel_Application();

		void Visualizate_Window();
		bool Is_Active();
		bool Draw_Panel_Application();

  private:

	bool active_window;

};

#endif // !_PANEL_APPLICATION_H_

