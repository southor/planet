#include "Gui.h"

namespace Prototype
{

	Gui::Gui()
	{
	}

	Gui::~Gui()
	{
		/*
		 * Destroy Guichan stuff
		 */
		delete font;
		delete gui;

		/*
		 * Widgets
		 */
		delete top;
		delete label;
		delete icon;
		delete button;
		delete textField;
		delete textBox;
		delete textBoxScrollArea;
		delete listBox;
		delete dropDown;
		delete checkBox1;
		delete checkBox2;
		delete radioButton1;
		delete radioButton2;
		delete radioButton3;
		delete slider;
		delete window;
		delete darkbitsIcon;
		delete darkbitsImage;
		delete nestedScrollArea;
		delete nestedContainer;
		delete nestedSlider;

		/*
		 * Destroy Guichan SDL stuff
		 */
		delete input;
		delete graphics;
		delete imageLoader;
	}

	void Gui::init()
	{
		/*
		 * Now it's time for Guichan OpenGL/SDL stuff
		 */
		imageLoader = new gcn::OpenGLSDLImageLoader();

		// The ImageLoader in use is static and must be set to be
		// able to load images
		gcn::Image::setImageLoader(imageLoader);
			graphics = new gcn::OpenGLGraphics();
		// We need to tell OpenGL graphics how big the screen is.
		graphics->setTargetPlane(640, 480);
		input = new gcn::SDLInput();

		/*
		 * Last but not least it's time to initialize and create the gui
		 * with Guichan stuff.
		 */
		top = new gcn::Container();
		// Set the dimension of the top container to match the screen.
		top->setDimension(gcn::Rectangle(0, 0, 640, 480));
		gui = new gcn::Gui();
		// Set gui to use the SDLGraphics object.
		gui->setGraphics(graphics);
		// Set gui to use the SDLInput object
		gui->setInput(input);
		// Set the top container
		gui->setTop(top);
		// Load the image font.
		font = new gcn::ImageFont("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		// The global font is static and must be set.
		gcn::Widget::setGlobalFont(font);

		initWidgets();
	}

	void Gui::initWidgets()
	{
		/*
		 * Create all the widgets
		 */
		label = new gcn::Label("Label");

		image = gcn::Image::load("gui-chan.bmp");
		icon = new gcn::Icon(image);

		button = new gcn::Button("Button");

		textField = new gcn::TextField("Text field");

		textBox = new gcn::TextBox("Multiline\nText box");
		textBoxScrollArea = new gcn::ScrollArea(textBox);
		textBoxScrollArea->setWidth(200);
		textBoxScrollArea->setHeight(100);
		textBoxScrollArea->setBorderSize(1);

		listBox = new gcn::ListBox(&demoListModel);
		listBox->setBorderSize(1);

		dropDown = new gcn::DropDown(&demoListModel);

		checkBox1 = new gcn::CheckBox("Checkbox 1");
		checkBox2 = new gcn::CheckBox("Checkbox 2");

		radioButton1 = new gcn::RadioButton("RadioButton 1", "radiogroup", true);
		radioButton2 = new gcn::RadioButton("RadioButton 2", "radiogroup");
		radioButton3 = new gcn::RadioButton("RadioButton 3", "radiogroup");

		slider = new gcn::Slider(0, 10);
		slider->setSize(100, 10);

		window = new gcn::Window("I am a window  Drag me");
		window->setBaseColor(gcn::Color(255, 150, 200, 190));

		darkbitsImage = gcn::Image::load("darkbitslogo_by_haiko.bmp");
		darkbitsIcon = new gcn::Icon(darkbitsImage);
		window->add(darkbitsIcon);
		window->resizeToContent();

		nestedSlider = new gcn::Slider(0, 10);
		nestedSlider->setSize(100, 10);
		
		nestedContainer = new gcn::Container();
		nestedContainer->setSize(400, 200);
		nestedContainer->add(nestedSlider, 50, 70);

		nestedScrollArea = new gcn::ScrollArea(nestedContainer);
		nestedScrollArea->setSize(180, 90);
		nestedScrollArea->setBorderSize(1);

		/*
		 * Add them to the top container
		 */
		top->add(label, 10, 10);
		top->add(icon, 10, 30);
		top->add(button, 200, 10);
		top->add(textField, 250, 10);
		top->add(textBoxScrollArea, 200, 50);
		top->add(listBox, 200, 200);
		top->add(dropDown, 500, 10);
		top->add(checkBox1, 500, 130);
		top->add(checkBox2, 500, 150);
		top->add(radioButton1, 500, 200);
		top->add(radioButton2, 500, 220);
		top->add(radioButton3, 500, 240);
		top->add(slider, 500, 300);
		top->add(window, 100, 350);
		top->add(nestedScrollArea, 440, 350);
	}
};
