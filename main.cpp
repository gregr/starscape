// main.cpp
// Gregory Rosenblatt
// 5/20/05

#define USE_PYTHON_TERMINAL
#define USE_CHAT_TERMINAL
#define USE_FONT_BOX

#include <iostream>
#ifdef USE_FONT_BOX
#include "gui/common/fontbox.h"
#endif
#include "app/application.h"
#ifdef USE_CHAT_TERMINAL
#include "net/terminal.h"
#endif
#ifdef USE_PYTHON_TERMINAL
#include "python/terminal.h"
#endif
#include "gui/control/list/listbox.h"
#include "gui/interface.h"
#include "gui/control/button/switchbutton.h"
#include "gui/control/button/commandbutton.h"
#include "gui/control/scroll/scrollfixture.h"
#include "gui/appearance/scroll/customsliderbrush.h"
#include "gui/appearance/button/textbuttonbrush.h"
#include "gui/appearance/button/plainbuttonbrush.h"
#include "gui/appearance/button/beveledbuttonbrush.h"
#include "gui/appearance/button/beveledimagebuttonbrush.h"
//#include "gui/control/edit/editline.h"
#include "gui/control/edit/label.h"
#include "gui/control/edit/documentmarkup.h"
#include "gui/clipboardstring.h"
#include "graphics/perspectiveview.h"
//#include "profiler/profilelogger.h"
#include "asciihextoint.h"
#include <fstream>
#include <sstream>
#include <list>
using namespace std;
using namespace Starscape;
using namespace Starscape::Raster;
using namespace Starscape::Graphics;
using namespace Starscape::System;
using namespace Starscape::Gui;

#define APP_CAPTION	"Starscape Application"
#define APP_WIDTH	640
#define APP_HEIGHT	480

#define CLIPBOARD_FILE	"clipboard.txt"
#define CURSOR_FILE	"defaultcursor.txt"
#define EDIT_CURSOR_FILE	"editcursor.txt"
#define FONT_FILE	"defaultfont.txt"
#define DOCBRUSH_FILE	"defaultdocument.txt"
#define EDITBOX_FILE	"defaulteditbox.txt"
#define BUTTON_FILE	"defaultbutton.txt"
#define VSCROLL_FILE	"verticalscrollbar.txt"
#define HSCROLL_FILE	"horizontalscrollbar.txt"
#define LISTBOX_FILE	"defaultlistbox.txt"
#define LISTITEM_BUTTON_FILE	"defaultlistitembutton.txt"
#define LISTCONTENTS_FILE	"listcontents.txt"

#define PYTHON_KEYWORD_FILE	"pythonkeywords.txt"

#define CURRENT_FPS_LABEL	"Current FPS: "

#define RETARDED_LABEL_TEXT	"Press 'F' for FPS info!"

bool globalDrawTriangle	= false;

const double PI = 180;
const float ANG_VELOCITY_MAX = 0.2f;
const float ANG_VELOCITY_MIN = -ANG_VELOCITY_MAX;
float angVelocity = 0.05f;

float angPos = 0.0f;

void AngVelInc() {
	angVelocity += 0.01f;
	if (angVelocity > ANG_VELOCITY_MAX)
		angVelocity = ANG_VELOCITY_MAX;
}

void AngVelDec() {
	angVelocity -= 0.01f;
	if (angVelocity < ANG_VELOCITY_MIN)
		angVelocity = ANG_VELOCITY_MIN;
}

void AngVelSet(double ratio) {
	assert(ratio >= 0.0);
	assert(ratio <= 1.0);
	angVelocity = ANG_VELOCITY_MIN
			+(ratio*(ANG_VELOCITY_MAX - ANG_VELOCITY_MIN));
}

double AngVelRefresh() {
	return ((angVelocity-ANG_VELOCITY_MIN)
			/(ANG_VELOCITY_MAX - ANG_VELOCITY_MIN));
}

void ToggleDrawTriangle() {
	globalDrawTriangle = !globalDrawTriangle;
}

void RenderStuff(const DisplayMetrics& metrics, unsigned int deltaTime=0) {
	if (!globalDrawTriangle)
		return;
//	const float angVelocity = 0.05f;
	PerspectiveView view(metrics);
	glTranslatef(0.0f, 0.0f, -5.0f);
	angPos += angVelocity*deltaTime;
	if (angPos > (PI+PI))
		angPos -= (PI+PI);
	else if (angPos < 0.0f)
		angPos += (PI+PI);
	glRotatef(angPos, 0.0f, 1.0f, 0.0f);
	EnableCulling();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.75f, -1.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.75f, -1.0f, 0.0f);

	// draw side that is usually culled CCW
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.75f, -1.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-0.75f, -1.0f, 0.0f);
	glEnd();
	DisableCulling();
}

typedef PooledList<PooledString>::Type	StringList;

typedef	ListBox<boost::shared_ptr<int> >	ListBoxType;

void LoadListContents(ListBoxType& list
					,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad list contents file";
	PooledString name, val;
	getline(fin, name);
	getline(fin, val);
	while (fin) {
		list.AddItem(name.c_str()
					,boost::shared_ptr<int>(new int(std::atoi(val.c_str()))));
		getline(fin, name);
		getline(fin, val);
	}
}

class TestScene : public Interface {
public:
	TestScene(const DisplayMetrics& metrics, AppearanceManager& appearance
			,const CursorBrush& cursorBrush
			,const ScrollController& controller
			,const ScrollBarBrush& vScrollBrush
			,const ScrollBarBrush& hScrollBrush
			,const ButtonBrush& buttonBrush, const Command& buttonCommand
			,const EditBoxBrush& editBrush, const EditBoxBrush& chatBrush
			,const ListBoxBrush& listBoxBrush)
		: Interface(metrics, appearance)
#ifdef USE_CHAT_TERMINAL
		,chatTerminal_(vScrollBrush, hScrollBrush, chatBrush
					,Net::GetOutputLogger())
#endif
#ifdef USE_PYTHON_TERMINAL
		,pythonDlg_(vScrollBrush, hScrollBrush, editBrush
					,Python::GetOutputLogger())
#endif
#ifdef USE_FONT_BOX
		,fontBox_(chatBrush, buttonBrush)
#endif
		,cursorBrush_(cursorBrush)
		,scrollBar_(vScrollBrush, controller)
		,testButton_(buttonBrush, buttonCommand, "Do Not Press This Button")
#ifdef USE_FONT_BOX
		,toggleFontBox_(buttonBrush
					,boost::bind(&TestScene::ToggleFontBox, this)
					,"Toggle FontBox")
#endif
		,wrapButton_(buttonBrush
					,boost::bind(&TestScene::SetTextWrapping, this, _1)
					,"Wrap Text")
		,label_(editBrush.GetDocumentBrush(), editBrush.GetDocumentMarkup()
				,RETARDED_LABEL_TEXT)
		,listBox_(listBoxBrush)
		,lastTime_(0), lastTime2_(0), framesRendered_(0)
		,totalTime_(0), updateFPS_(false)
	{
		SetDefaultCursorBrush(cursorBrush_);
		AddChild(scrollBar_);
		scrollBar_.SetPos(10, 10);
		scrollBar_.SetLength(200);
		AddChild(testButton_);
		testButton_.SetPos(50, 450);
#ifdef USE_FONT_BOX
		AddChild(toggleFontBox_);
		toggleFontBox_.SetPos(50, 430);
#endif
		AddChild(wrapButton_);
		wrapButton_.SetPos(50, 410);
#ifdef USE_PYTHON_TERMINAL
		AddChild(pythonDlg_);
		pythonDlg_.SetPos(250, 20);
#endif
#ifdef USE_CHAT_TERMINAL
		AddChild(chatTerminal_);
		chatTerminal_.SetPos(250, 250);
#endif
#ifdef USE_FONT_BOX
		AddChild(fontBox_);
		fontBox_.SetPos(0, 0);
		fontBox_.Hide();
#endif
		AddChild(listBox_);
		listBox_.SetPos(50, 50);
		LoadListContents(listBox_, LISTCONTENTS_FILE);
//		listBox_.AddItem("i am a listbox", ListBox<int>::DataPtr(new int(42)));
//		listBox_.AddItem("hello listbox", ListBox<int>::DataPtr(new int(17)));
//		listBox_.AddItem("abcdefg", ListBox<int>::DataPtr(new int(21)));
//		cout << "listbox item added" << endl;
		listBox_.Sort();
		listBox_.OnResize();
//		listBox_.ReverseSort();
	}
	void OnKeyDown(const Input::KeyInfo& key) {
		if (key.sym == 'f')
			updateFPS_ = true;
		else if (key.sym == 's')
			ShowCursor();
		else if (key.sym == 'h')
			HideCursor();
		else if (key.sym == SDLK_ESCAPE)
			PostQuitEvent();
	}
	void OnUpdate(unsigned int deltaTime) {
		lastTime2_ = totalTime_;
		totalTime_+=deltaTime;
		if (updateFPS_) {
			UpdateFPS();
			updateFPS_ = false;
		}
	}
	void ToggleFontBox() {
#ifdef USE_FONT_BOX
		if (fontBox_.IsVisible())
			fontBox_.Hide();
		else
			fontBox_.Show();
#endif
	}
	void SetTextWrapping(bool wrap) {
		if (wrap) {
#ifdef USE_CHAT_TERMINAL
			chatTerminal_.EnableTextWrapping();
#endif
#ifdef USE_PYTHON_TERMINAL
			pythonDlg_.EnableTextWrapping();
#endif
		}
		else {
#ifdef USE_CHAT_TERMINAL
			chatTerminal_.DisableTextWrapping();
#endif
#ifdef USE_PYTHON_TERMINAL
			pythonDlg_.DisableTextWrapping();
#endif
		}
	}
protected:
	void UpdateFPS() {
		unsigned int deltaTime = totalTime_ - lastTime_;
		double fps = static_cast<double>(framesRendered_*1000) / deltaTime;
		PooledOutputStringStream ss("");
		ss << CURRENT_FPS_LABEL;
		ss << fps;
		label_.SetString(ss.str().c_str());
		framesRendered_ = 0;
		lastTime_ = totalTime_;
	}
	void OnRender() {
		UnbindTextures();
		RenderStuff(GetDisplayMetrics(), (totalTime_-lastTime2_));
		label_.Draw(200, 210);
		++framesRendered_;
	}
private:
#ifdef USE_CHAT_TERMINAL
	Net::Terminal	chatTerminal_;
#endif
#ifdef USE_PYTHON_TERMINAL
	Python::Terminal	pythonDlg_;
#endif
#ifdef USE_FONT_BOX
	FontBox			fontBox_;
#endif
	CursorBrush		cursorBrush_;
	VScrollBar		scrollBar_;
	CommandButton	testButton_;
#ifdef USE_FONT_BOX
	CommandButton	toggleFontBox_;
#endif
	SwitchButton	wrapButton_;
	Label			label_;
	ListBoxType		listBox_;
	int				lastTime_, lastTime2_, framesRendered_;
	unsigned int	totalTime_;
	bool			updateFPS_;
};

void LoadClipboard(const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw Exception("bad message file");
	SetClipboardFromStream(fin);
}

void SaveClipboard(const char* fileName) {
	ofstream fout(fileName);
	fout << GetClipboardString();
}

ImageFontHandle MakeFont(AppearanceManager& appearance
						,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw Exception("bad font file");
	PooledString fontName;
	int fontSize;
	ImageFont::Quality	quality = ImageFont::SOLID_TEXT;
	getline(fin, fontName);
//	cout << fontName << endl;
	fin >> fontSize;
//	cout << fontSize << endl;
	int qualflag = 0;
	fin >> qualflag;
//	cout << qualflag << endl;
	if (qualflag!=0)
		quality = ImageFont::BLENDED_TEXT;
	return appearance.GetFont(fontName.c_str(), fontSize, quality);
}

CursorBrush MakeCursor(AppearanceManager& appearance
						,const char* fileName) {
	ifstream cursorFile(fileName);
	if (!cursorFile)
		throw Exception("bad cursor file");
	PooledString cursorBrushFile;
	getline(cursorFile, cursorBrushFile);
	int useKey;
	cursorFile >> useKey;
	SDL_Color colorKey = MakeSurfaceColorFromStream(cursorFile);
	SDL_Rect cursorRegion = MakeRectFromStream(cursorFile);
	Coord activePos;
	cursorFile >> activePos.x;
	cursorFile >> activePos.y;
	ImageHandle img = appearance.GetImage(cursorBrushFile.c_str()
								,(useKey!=0), colorKey);
	return CursorBrush(StaticImage(img, ImageRegion(*img, cursorRegion))
						,activePos);
}

class ScrollBarAppearance {
public:
	ScrollBarAppearance(const BeveledImage& incButtonImage
						,const BeveledImage& decButtonImage
						,Color color)
		: incButtonBrush_(incButtonImage), decButtonBrush_(decButtonImage)
		,sliderBrush_(color, incButtonImage.Width(), incButtonImage.Height())
		,trackBrush_(sliderBrush_, GetLighterColor(color))
		,scrollBrush_(trackBrush_, incButtonBrush_, decButtonBrush_)	{}
	const ScrollBarBrush& GetScrollBarBrush() const	{ return scrollBrush_; }
private:
	BeveledImageButtonBrush incButtonBrush_;
	BeveledImageButtonBrush decButtonBrush_;
	PlainSliderBrush	sliderBrush_;
	SliderTrackBrush	trackBrush_;
	ScrollBarBrush		scrollBrush_;
};

ScrollBarAppearance MakeScrollBarAppearance(AppearanceManager& appearance
											,const char* fileName) {
	ifstream scrollFile(fileName);
	if (!scrollFile)
		throw Exception("bad scrollbar file");
	PooledString scrollImageFile;
	getline(scrollFile, scrollImageFile);
	int scrollUseKey;
	scrollFile >> scrollUseKey;
	SDL_Color scrollColorKey = MakeSurfaceColorFromStream(scrollFile);
	SDL_Rect regioninc = MakeRectFromStream(scrollFile);
	SDL_Rect regiondec = MakeRectFromStream(scrollFile);
	ImageHandle scrollSkinImage = appearance.GetImage(scrollImageFile.c_str()
										,(scrollUseKey!=0), scrollColorKey);
	ImageRegion incRegion(*scrollSkinImage, regioninc);
	ImageRegion decRegion(*scrollSkinImage, regiondec);
	Color scrollColor = MakeColorFromStream(scrollFile);
	BeveledImage incButtonImage(StaticImage(scrollSkinImage, incRegion)
								,scrollColor);
	BeveledImage decButtonImage(StaticImage(scrollSkinImage, decRegion)
								,scrollColor);
	return ScrollBarAppearance(incButtonImage, decButtonImage, scrollColor);
}

DocumentBrush MakeDocBrush(const ImageFontHandle& font
							,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad docbrush file";
	Color fontColor = MakeColorFromStream(fin);
	Color selectTextColor = MakeColorFromStream(fin);
	Color caretColor = MakeColorFromStream(fin);
	Color selectColor = MakeColorFromStream(fin);
	return DocumentBrush(font, fontColor, selectTextColor
						,caretColor, selectColor);
}

EditBoxBrush MakeEditBoxBrush(const DocumentBrush& docBrush
							,const DocumentMarkup& markup
							,const CursorBrush& editCursor
							,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad editbox file";
	Color backgroundColor = MakeColorFromStream(fin);
	return EditBoxBrush(docBrush, markup, editCursor, backgroundColor);
}

typedef TextButtonBrush<BeveledButtonBrush>	BeveledTextButtonBrush;

BeveledTextButtonBrush MakeButtonBrush(const ImageFontHandle& font
									,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad button file";
	Color frameColor = MakeColorFromStream(fin);
	Color textColor = MakeColorFromStream(fin);
	SizeScalar marginWidth, marginHeight;
	fin >> marginWidth >> marginHeight;
	return BeveledTextButtonBrush(BeveledButtonBrush(frameColor)
								,textColor, font
								,marginWidth, marginHeight);
}

typedef TextButtonBrush<PlainButtonBrush> PlainTextButtonBrush;

PlainTextButtonBrush MakeListItemButtonBrush(const ImageFontHandle& font
											,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad listitem button file";
	Graphics::Color unpressedColor = MakeColorFromStream(fin);
	Graphics::Color pressedColor = MakeColorFromStream(fin);
	Graphics::Color hoverColor = MakeColorFromStream(fin);
	Graphics::Color textColor = MakeColorFromStream(fin);
	SizeScalar marginWidth, marginHeight;
	fin >> marginWidth >> marginHeight;
	return PlainTextButtonBrush(PlainButtonBrush(unpressedColor
												,pressedColor, hoverColor)
								,textColor, font, marginWidth, marginHeight);
}

ListBoxBrush MakeListBoxBrush(const ButtonBrush& buttonBrush
							,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad listbox file";
	Graphics::Color bgColor = MakeColorFromStream(fin);
	return ListBoxBrush(buttonBrush, bgColor);
}

void LoadKeywords(KeywordMarkup::KeywordColorMap& keywords
				,const char* fileName) {
	ifstream fin(fileName);
	if (!fin)
		throw "bad python-keyword file";
	PooledString word;
	PooledString colorVal;
	fin >> word >> colorVal;
	while (fin) {
		keywords[word] = AsciiHexToInt(colorVal.c_str());
		fin >> word >> colorVal;
	}
}

int main(int argc, char* argv[]) {
	try {
		LoadClipboard(CLIPBOARD_FILE);
		Application application(APP_CAPTION, APP_WIDTH, APP_HEIGHT);
		AppearanceManager appearance;
		ImageFontHandle imageFont = MakeFont(appearance, FONT_FILE);
		CursorBrush cursorBrush = MakeCursor(appearance, CURSOR_FILE);
		CursorBrush editCursorBrush = MakeCursor(appearance, EDIT_CURSOR_FILE);
		DocumentBrush docBrush = MakeDocBrush(imageFont, DOCBRUSH_FILE);
		KeywordMarkup::KeywordColorMap keywords;
		LoadKeywords(keywords, PYTHON_KEYWORD_FILE);
		KeywordMarkup docMarkup(keywords);
		TagMarkup chatMarkup;
		EditBoxBrush editBrush = MakeEditBoxBrush(docBrush, docMarkup
												,editCursorBrush
												,EDITBOX_FILE);
		EditBoxBrush chatBrush = MakeEditBoxBrush(docBrush, chatMarkup
												,editCursorBrush
												,EDITBOX_FILE);
		BeveledTextButtonBrush buttonBrush = MakeButtonBrush(imageFont
														,BUTTON_FILE);
		Command buttonCommand(boost::bind(&ToggleDrawTriangle));
		// v scrollbar stuff here
		ScrollBarAppearance vScrollAppearance =
				MakeScrollBarAppearance(appearance, VSCROLL_FILE);
		// h scrollbar stuff here
		ScrollBarAppearance hScrollAppearance =
				MakeScrollBarAppearance(appearance, HSCROLL_FILE);
		// listbox
		PlainTextButtonBrush listItemButtonBrush =
				MakeListItemButtonBrush(imageFont, LISTITEM_BUTTON_FILE);
		ListBoxBrush listBoxBrush = MakeListBoxBrush(listItemButtonBrush
													,LISTBOX_FILE);
		// controller
		boost::function<void ()> funcinc = &AngVelInc;
		boost::function<void ()> funcdec = &AngVelDec;
		boost::function<void (double)> funcslide = &AngVelSet;
		boost::function<double ()> funcrefresh = &AngVelRefresh;
		ScrollController scrollController(funcinc, funcdec
										,funcslide, funcrefresh);
//		cout << "before scene" << endl;
		// scene
		TestScene scene(application.GetDisplayMetrics()
				,appearance, cursorBrush
				,scrollController, vScrollAppearance.GetScrollBarBrush()
				,hScrollAppearance.GetScrollBarBrush()
				,buttonBrush, buttonCommand
				,editBrush, chatBrush, listBoxBrush);
//		cout << "got here?" << endl;
		application.Run(scene);
		SaveClipboard(CLIPBOARD_FILE);
	}
	catch (...) {
		HandleException();
	}
	return 0;
}
