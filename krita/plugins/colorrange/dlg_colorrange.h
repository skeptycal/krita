/*
 *  dlg_colorrange.h -- part of KimageShop^WKrayon^WKrita
 *
 *  Copyright (c) 2004 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 */
#ifndef DLG_COLORRANGE
#define DLG_COLORRANGE

#include <qpixmap.h>
#include <qcolor.h>
#include <qcursor.h>

#include <kdialogbase.h>

#include <kis_types.h>

#include <kis_brush.h>
#include <kis_canvas_observer.h>
#include <kis_canvas_subject.h>
#include <kis_cursor.h>
#include <kis_filter.h>
#include <kis_filter_registry.h>
#include <kis_gradient.h>
#include <kis_id.h>
#include <kis_iterators_pixel.h>
#include <kis_layer.h>
#include <kis_paint_device.h>
#include <kis_pattern.h>
#include <kis_selection.h>
#include <kis_selection_manager.h>
#include <kis_transaction.h>
#include <kis_tool_controller.h>
#include <kis_tool.h>
#include <kis_tool_registry.h>
#include <kis_types.h>
#include <kis_undo_adapter.h>
#include <kis_view.h>

#include "wdg_colorrange.h"


class KisTool;
class KisView;
class KisCanvasSubject;
class DlgColorRange;

// A little hack around CanvasSubject to make sure we get the colour, and not the view.
class ColorRangeCanvasSubject: public KisCanvasSubject  {
	
public:
		
	ColorRangeCanvasSubject(DlgColorRange * parent, KisView * view) : m_parent(parent), m_view(view) { m_subject = m_view -> getCanvasSubject(); };
	virtual ~ColorRangeCanvasSubject() {};

	virtual void attach(KisCanvasObserver *observer) { observer -> update(this); };
	virtual void detach(KisCanvasObserver *observer) { observer -> update(this); };
	virtual void notify() { m_subject -> notify() ;};
	virtual KisImageSP currentImg() const { return m_subject -> currentImg(); };
	virtual QString currentImgName() const { return m_subject -> currentImgName(); };
	virtual QColor bgColor() const { return QColor(); };
	virtual void setBGColor(const QColor& c);
	virtual QColor fgColor() const { return QColor(); };
	virtual void setFGColor(const QColor& c);
	virtual KisBrush *currentBrush() const { return 0; };
	virtual KisPattern *currentPattern() const { return 0; };
	virtual KisGradient *currentGradient() const { return 0; };
	virtual double zoomFactor() const { return m_subject -> zoomFactor(); };
	virtual KisUndoAdapter *undoAdapter() const { return m_subject -> undoAdapter(); };
	virtual KisCanvasControllerInterface *canvasController() const { return m_subject -> canvasController(); };
	virtual KisToolControllerInterface * toolController() const { return m_subject -> toolController(); };
	virtual KoDocument * document() const { return m_subject -> document() ; };
	virtual QCursor setCanvasCursor(const QCursor & cursor) { return m_subject-> setCanvasCursor(cursor); };
	virtual KisProgressDisplayInterface *progressDisplay() const { return m_subject-> progressDisplay(); };
	virtual KisSelectionManager * selectionManager() { return m_subject -> selectionManager(); };
	virtual KisFilterRegistry * filterRegistry() const { return m_subject -> filterRegistry(); };
	virtual KisFilterSP filterGet(const KisID& id) { return m_subject -> filterGet(id); };
	virtual KisIDList filterList() {return m_subject -> filterList(); };

private:

	DlgColorRange * m_parent;
	KisView * m_view;
	KisCanvasSubject * m_subject;
 
};

enum enumPickMode {
	PICK,
	PICK_PLUS,
	PICK_MINUS,
};
	

 /**
 * This dialog allows the user to create a selection mask based
 * on a (range of) colors.
 */
class DlgColorRange: public KDialogBase {
	typedef KDialogBase super;
	Q_OBJECT



public:

	DlgColorRange(KisView * view, KisLayerSP layer, QWidget * parent = 0, const char* name = 0);
	~DlgColorRange();

public slots:
	void slotColorChanged(const QColor & c);

private slots:

	void okClicked();
	void cancelClicked();
	
	void slotPickerPlusClicked();
	void slotPickerClicked();
	void slotLoad();
	void slotPickerMinusClicked();
	void slotSave();
	void slotInvertClicked();
	void slotFuzzinessChanged(int value);
	void slotSliderMoved(int value);
	void slotSelectionTypeChanged(int index);
	void slotPreviewTypeChanged(int index);
	void updatePreview();

	
private:
	QImage createMask(KisSelectionSP selection, KisLayerSP layer);
	
private:

	WdgColorRange * m_page;
	KisSelectionSP m_selection;
	KisLayerSP m_layer;
	KisView * m_view;
	KisCanvasSubject * m_subject;
	KisTool * m_picker;
	ColorRangeCanvasSubject * m_canvasSubject;
	int m_pickMode;
	QCursor m_oldCursor;
	KisTransaction * m_transaction;
};

#endif // DLG_COLORRANGE
