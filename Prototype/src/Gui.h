#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
// A class used to load images for OpenGL using SDL
#include <guichan/opengl/openglsdlimageloader.hpp>


namespace Prototype
{
	/*
	 * List boxes and dropdowns needs an instance of a listmodel
	 * to know what elements they have.
	 */
	class DemoListModel : public gcn::ListModel
	{
	public:
		int getNumberOfElements()
		{
			return 5;
		}

		std::string getElementAt(int i)
		{
			switch(i)
			{
				case 0:
					return std::string("zero");
				case 1:
					return std::string("one");
				case 2:
					return std::string("two");
				case 3:
					return std::string("three");
				case 4:
					return std::string("four");
				default: // Just to keep warnings away
					return std::string("");
			}
		}
	};




	class Gui
	{
	public:
		Gui();
		~Gui();
	
		void init();
		void initWidgets();

		
		// Guichan
		gcn::SDLInput* input;                    // Input driver
		gcn::OpenGLGraphics* graphics;           // Graphics driver
		gcn::OpenGLSDLImageLoader* imageLoader;  // For loading images

		/*
		 * Guichan stuff we need
		 */
		gcn::Gui* gui;            // A Gui object - binds it all together
		gcn::ImageFont* font;     // A font

		/*
		 * All of the default widgets
		 */
		gcn::Container* top;                 // A top container
		gcn::Label* label;                   // A label
		gcn::Icon* icon;                     // An icon (image)
		gcn::Button* button;                 // A button
		gcn::TextField* textField;           // One-line text field
		gcn::TextBox* textBox;               // Multi-line text box
		gcn::ScrollArea* textBoxScrollArea;  // Scroll area for the text box
		gcn::ListBox* listBox;               // A list box
		gcn::DropDown* dropDown;             // Drop down
		gcn::CheckBox* checkBox1;            // Two checkboxes
		gcn::CheckBox* checkBox2;
		gcn::RadioButton* radioButton1;      // Three radio buttons
		gcn::RadioButton* radioButton2;
		gcn::RadioButton* radioButton3;
		gcn::Slider* slider;                 // A slider
		gcn::Window *window;
		gcn::ScrollArea* nestedScrollArea;
		gcn::Container* nestedContainer;
		gcn::Slider* nestedSlider;
		
		DemoListModel demoListModel;

	};
};

