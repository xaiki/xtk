
/* GTK - The GIMP Toolkit
 *
 * Copyright (C) 2009 Matthias Clasen <mclasen@redhat.com>
 * Copyright (C) 2008 Richard Hughes <richard@hughsie.com>
 * Copyright (C) 2009 Bastien Nocera <hadess@hadess.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.         See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*
#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif
*/
#ifndef __XTK_CELL_RENDERER_WIDGET_H__
#define __XTK_CELL_RENDERER_WIDGET_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTK_TYPE_CELL_RENDERER_WIDGET            (xtk_cell_renderer_widget_get_type ())
#define XTK_CELL_RENDERER_WIDGET(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_WIDGET, XtkCellRendererWidget))
#define XTK_CELL_RENDERER_WIDGET_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_WIDGET, XtkCellRendererWidgetClass))
#define GTK_IS_CELL_RENDERER_WIDGET(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_WIDGET))
#define GTK_IS_CELL_RENDERER_WIDGET_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_WIDGET))
#define XTK_CELL_RENDERER_WIDGET_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_WIDGET, XtkCellRendererWidgetClass))

typedef struct _XtkCellRendererWidget        XtkCellRendererWidget;
typedef struct _XtkCellRendererWidgetClass   XtkCellRendererWidgetClass;
typedef struct _XtkCellRendererWidgetPrivate XtkCellRendererWidgetPrivate;

struct _XtkCellRendererWidget
{
  GtkCellRenderer                parent;

  /*< private >*/
  XtkCellRendererWidgetPrivate *priv;
};

struct _XtkCellRendererWidgetClass
{
  GtkCellRendererClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType            xtk_cell_renderer_widget_get_type (void) G_GNUC_CONST;
GtkCellRenderer *xtk_cell_renderer_widget_new      (void);

static void
xtk_cell_renderer_widget_class_init (XtkCellRendererWidgetClass *klass);
static void
xtk_cell_renderer_widget_init (XtkCellRendererWidget *cell);

G_END_DECLS

#endif /* __XTK_CELL_RENDERER_WIDGET_H__ */
