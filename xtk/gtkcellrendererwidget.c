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
 * Modified by the GTK+ Team and others 2007.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */
/*
#include "config.h"

#include "gtkcellrendererwidget.h"
#include "gtkiconfactory.h"
#include "gtkicontheme.h"
#include "gtkintl.h"
#include "gtksettings.h"
#include "gtktypebuiltins.h"

#undef GDK_DEPRECATED
#undef GDK_DEPRECATED_FOR
#define GDK_DEPRECATED
#define GDK_DEPRECATED_FOR(f)

#include "deprecated/gtkstyle.h"
*/

#include "gtkcellrendererwidget.h"
#include "gtk/gtk.h"

/**
 * SECTION:xtkcellrendererwidget
 * @Short_description: Renders a spinning animation in a cell
 * @Title: Xtkcellrendererwidget
 * @See_also: #GtkWidget, #GtkCellRendererProgress
 *
 * Xtkcellrendererwidget renders a spinning animation in a cell, very
 * similar to #GtkWidget. It can often be used as an alternative
 * to a #GtkCellRendererProgress for displaying indefinite activity,
 * instead of actual progress.
 *
 * To start the animation in a cell, set the #XtkCellRendererWidget:active
 * property to %TRUE and increment the #XtkCellRendererWidget:pulse property
 * at regular intervals. The usual way to set the cell renderer properties
 * for each cell is to bind them to columns in your tree model using e.g.
 * gtk_tree_view_column_add_attribute().
 */

#define xatrace() printf("%s:%d:%s():\n" , __FILE__, __LINE__, __func__)

enum {
  PROP_0,
  PROP_WIDGET,
  PROP_ACTIVE,
  PROP_PULSE,
  PROP_SIZE
};

struct _XtkCellRendererWidgetPrivate
{
  GHashTable *whash;
	GtkWidget *widget;
};


static void xtk_cell_renderer_widget_get_property (GObject         *object,
                                                    guint            param_id,
                                                    GValue          *value,
                                                    GParamSpec      *pspec);
static void xtk_cell_renderer_widget_set_property (GObject         *object,
                                                    guint            param_id,
                                                    const GValue    *value,
                                                    GParamSpec      *pspec);
static void xtk_cell_renderer_widget_get_size     (GtkCellRenderer *cell,
                                                    GtkWidget          *widget,
                                                    const GdkRectangle *cell_area,
                                                    gint               *x_offset,
                                                    gint               *y_offset,
                                                    gint               *width,
                                                    gint               *height);
static GtkCellEditable *
xtk_cell_renderer_widget_start_editing (GtkCellRenderer     *cell,
                                       GdkEvent            *event,
                                       GtkWidget           *widget,
                                       const gchar         *path,
                                       const GdkRectangle  *background_area,
                                       const GdkRectangle  *cell_area,
                                       GtkCellRendererState flags);

static void xtk_cell_renderer_widget_render       (GtkCellRenderer      *cell,
                                                    cairo_t              *cr,
                                                    GtkWidget            *widget,
                                                    const GdkRectangle   *background_area,
                                                    const GdkRectangle   *cell_area,
                                                    GtkCellRendererState  flags);

G_DEFINE_TYPE (XtkCellRendererWidget, xtk_cell_renderer_widget, GTK_TYPE_CELL_RENDERER)

static void
xtk_cell_renderer_widget_class_init (XtkCellRendererWidgetClass *klass)
{
  xatrace();
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkCellRendererClass *cell_class = GTK_CELL_RENDERER_CLASS (klass);

  object_class->get_property = xtk_cell_renderer_widget_get_property;
  object_class->set_property = xtk_cell_renderer_widget_set_property;

  cell_class->get_size = xtk_cell_renderer_widget_get_size;
  cell_class->render = xtk_cell_renderer_widget_render;
  cell_class->start_editing = xtk_cell_renderer_widget_start_editing;

  /* XtkCellRendererWidget:active:
   *
   * Whether the widget is active (ie. shown) in the cell
   *
   * Since: 2.20
   */
  g_object_class_install_property (object_class,
                                   PROP_ACTIVE,
                                   g_param_spec_boolean ("active",
                                                         "Active",
                                                         "Whether the widget is active (ie. shown) in the cell",
                                                         FALSE,
                                                         G_PARAM_READWRITE));
  /**
   * XtkCellRendererWidget:widget:
   *
   * the #GtkWidget to attach to this cell.
   * Ideally this should be automatic, but there is still lots of vodo for me there.
   *
   * Since: 3.6
   */
  g_object_class_install_property (object_class,
                                   PROP_WIDGET,
                                   g_param_spec_object ("widget",
                                                        "Widget",
                                                        "GtkWidget to attach",
                                                        GTK_TYPE_WIDGET,
                                                        G_PARAM_READWRITE));
  /**
   * XtkCellRendererWidget:size:
   *
   * The #GtkIconSize value that specifies the size of the rendered widget.
   *
   * Since: 2.20
   */
  g_object_class_install_property (object_class,
                                   PROP_SIZE,
                                   g_param_spec_enum ("size",
                                                      "Size",
                                                      "The GtkIconSize value that specifies the size of the rendered widget",
                                                      GTK_TYPE_ICON_SIZE, GTK_ICON_SIZE_MENU,
                                                      G_PARAM_READWRITE));


  g_type_class_add_private (object_class, sizeof (XtkCellRendererWidgetPrivate));
}

static gboolean
xtk_cell_renderer_widget_cell_draw (GtkWidget *widget,
                                    cairo_t   *cr,
                                    gpointer   user_data)
{
  GtkOffscreenWindow *offscreen = (GtkOffscreenWindow *)user_data;

  printf ("DRAWN\n");
  cairo_set_source_surface (cr,
                            gtk_offscreen_window_get_surface (offscreen),
                            50, 50);
  cairo_paint (cr);

  return FALSE;
}

static gboolean
xtk_cell_renderer_widget_cell_queue_redraw (GtkWidget *window,
                                            GdkEvent  *event,
                                            GtkWidget *treeview)
{
  printf ("DAMAGED\n");
  gtk_widget_queue_draw (treeview);

  return TRUE;
}
/*
static void
gtk_offscreen_window_damaged_cb (GtkWidget *window, GdkEvent *event, GtkWidget *cell)
{

  XtkCellRendererWidgetPrivate *priv = XTK_CELL_RENDERER_WIDGET(cell)->priv;

/*  if (priv->treeview)
    xtk_cell_renderer_widget_cell_queue_redraw (window, event, priv->treeview); * /

  if (! priv->hackimg)
    return;
  GdkPixbuf *p = gtk_offscreen_window_get_pixbuf (GTK_OFFSCREEN_WINDOW(priv->window));

  gtk_image_set_from_pixbuf (GTK_IMAGE (priv->hackimg), p);
}
*/
static void
xtk_cell_renderer_widget_init (XtkCellRendererWidget *cell)
{
  xatrace();

  cell->priv = G_TYPE_INSTANCE_GET_PRIVATE (cell,
                                            GTK_TYPE_CELL_RENDERER_WIDGET,
                                            XtkCellRendererWidgetPrivate);

  cell->priv->whash = g_hash_table_new(NULL, NULL);
}

/**
 * xtk_cell_renderer_widget_new:
 *
 * Returns a new cell renderer which will show a #GtkWidget.
 *
 * Return value: a new #GtkCellRenderer
 *
 * Since: 2.20
 */
GtkCellRenderer *
xtk_cell_renderer_widget_new (void)
{
  xatrace();  return g_object_new (GTK_TYPE_CELL_RENDERER_WIDGET, NULL);
}

static void
xtk_cell_renderer_widget_get_property (GObject    *object,
                                        guint       param_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
  xatrace();  XtkCellRendererWidget *cell = XTK_CELL_RENDERER_WIDGET (object);
  XtkCellRendererWidgetPrivate *priv = cell->priv;

  switch (param_id)
    {
    case PROP_WIDGET:
      g_value_set_object (value, priv->widget);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}

static gboolean
xtk_cell_renderer_widget_offscreen_draw (GtkWidget *widget,
                                         cairo_t   *cr,
                                         gpointer  userdata)
{
  cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 0.0); /* transparent */

  /* draw the background */
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  return FALSE;
}

static void
xtk_cell_renderer_widget_offscreen_destroy (GtkWidget *widget,
					    XtkCellRendererWidgetPrivate *priv)
{
	GtkWidget *window = g_hash_table_lookup (priv->whash, widget);
	g_assert(window);
	gtk_widget_destroy (window);
	g_hash_table_remove (priv->whash, widget);
}

static void
xtk_cell_renderer_widget_set_property (GObject      *object,
                                        guint         param_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
  xatrace();  XtkCellRendererWidget *cell = XTK_CELL_RENDERER_WIDGET (object);
  XtkCellRendererWidgetPrivate *priv = cell->priv;
  GtkWidget *window;

  switch (param_id)
    {
    case PROP_WIDGET:

      window = g_value_get_object (value);
      if (! window) {
        priv->widget = NULL;
        printf ("widget set to (nil)\n");
        break;
      }
/*      if (w == priv->widget) {
        printf ("widget already set\n");
        break;
        }*/

      priv->widget = window;

      window = g_hash_table_lookup (priv->whash, priv->widget);
      if (!window) {
        window = gtk_offscreen_window_new ();
        g_hash_table_insert (priv->whash, priv->widget, window);
        gtk_container_add (GTK_CONTAINER (window), priv->widget);
        gtk_widget_set_app_paintable (window, TRUE);

	g_signal_connect (priv->widget, "destroy",
			  G_CALLBACK (xtk_cell_renderer_widget_offscreen_destroy),
			  priv);

        g_signal_connect (G_OBJECT(window), "draw",
                          G_CALLBACK (xtk_cell_renderer_widget_offscreen_draw),
                          NULL);
        gtk_widget_show_all (window);
      }

      printf ("%p:%p, setting widget to: %p, on window: %p\n", object, priv, priv->widget, window);
      break;
    default:
	    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}

static void
xtk_cell_renderer_widget_get_size (GtkCellRenderer    *cellr,
                                    GtkWidget          *widget,
                                    const GdkRectangle *cell_area,
                                    gint               *x_offset,
                                    gint               *y_offset,
                                    gint               *width,
                                    gint               *height)
{
  xatrace();  XtkCellRendererWidget *cell = XTK_CELL_RENDERER_WIDGET (cellr);
  XtkCellRendererWidgetPrivate *priv = cell->priv;

  if (x_offset)
    *x_offset = 0;
  if (y_offset)
    *y_offset = 0;

  if (! priv->widget) {
    if (width)
      *width = 0;
    if (height)
      *height = 0;
    return;
  }

  GtkRequisition natural_size;
  GtkRequisition min;

  gtk_widget_get_preferred_size (priv->widget, &min, &natural_size);
  printf ("requesting: %dx%d\n", natural_size.width, natural_size.height);
  printf ("requesting (min): %dx%d\n", min.width, min.height);
  if (width)
    *width = natural_size.width;
  if (height)
    *height = natural_size.height;

  return;
}

#define XTK_CELL_RENDERER_WIDGET_PATH "gtk-cell-renderer-widget-path"

static GtkCellEditable *
xtk_cell_renderer_widget_start_editing (GtkCellRenderer     *cellr,
                                        GdkEvent            *event,
                                        GtkWidget           *treeview,
                                        const gchar         *path,
                                        const GdkRectangle  *background_area,
                                        const GdkRectangle  *cell_area,
                                        GtkCellRendererState flags)
{
  xatrace();  XtkCellRendererWidget *cell = XTK_CELL_RENDERER_WIDGET (cellr);
  XtkCellRendererWidgetPrivate *priv = cell->priv;
  GtkWidget *widget = priv->widget;

  if (!widget)
    printf ("NO WIDGET !!!!\n");

  g_object_set_data_full (G_OBJECT (widget),
                          XTK_CELL_RENDERER_WIDGET_PATH,
                          g_strdup (path), g_free);

  gtk_widget_show(widget);

  return GTK_CELL_EDITABLE (widget);
}

void xabreak(void) {};

inline static GtkWidget *
get_first_child (GtkWidget *p)
{
  return p==NULL?NULL:GTK_WIDGET(gtk_container_get_children (GTK_CONTAINER(p))->data);
}

static void
xtk_cell_renderer_widget_render (GtkCellRenderer      *cellr,
                                  cairo_t              *cr,
                                  GtkWidget            *treeview,
                                  const GdkRectangle   *background_area,
                                  const GdkRectangle   *cell_area,
                                  GtkCellRendererState  flags)
{
  xatrace();  XtkCellRendererWidget *cell = XTK_CELL_RENDERER_WIDGET (cellr);
  XtkCellRendererWidgetPrivate *priv = cell->priv;
  GtkWidget *widget = priv->widget;
  GtkWidget *window;
  GtkAllocation *alloc = (GtkAllocation *) cell_area;
  GtkStyleContext *context = gtk_widget_get_style_context (treeview);

  GdkRectangle pix_rect;
  GdkRectangle draw_rect;
  gint xpad, ypad;

  int ahah = 0;

  cairo_surface_t *cs = NULL;

  if (!widget)
    return;

  window = g_hash_table_lookup (priv->whash, widget);
  if (! window) {
	  printf ("No WINDOW for %p !!! how can this happen ?!!?\n", widget);
	  return;
  }

  xtk_cell_renderer_widget_get_size (cellr, treeview, (GdkRectangle *) cell_area,
                                      &pix_rect.x, &pix_rect.y,
                                      &pix_rect.width, &pix_rect.height);
  g_object_get (cellr,
                "xpad", &xpad,
                "ypad", &ypad,
                NULL);
  pix_rect.x += cell_area->x + xpad;
  pix_rect.y += cell_area->y + ypad;
  pix_rect.width -= xpad * 2;
  pix_rect.height -= ypad * 2;

  if (!gdk_rectangle_intersect (cell_area, &pix_rect, &draw_rect)) {
    printf ("nothing to draw, no intersection\n");
    return;
  }

  cs = gtk_offscreen_window_get_surface (GTK_OFFSCREEN_WINDOW(window));

  cairo_save (cr);

  gdk_cairo_rectangle (cr, cell_area);
  cairo_clip (cr);

  cairo_set_source_surface (cr, cs, draw_rect.x, draw_rect.y);
  printf (" +++  painting on: %p:%dx%d, with surface: %p\n", cr, draw_rect.x, draw_rect.y, cs);

  cairo_paint (cr);
  cairo_restore (cr);
}

GType xtk_cell_renderer_widget_register_type()
{
     static const GTypeInfo renderer_widget_info =
   {
     sizeof(struct _XtkCellRendererWidgetClass),
     0,    /* base_init */
     0,    /* base_finalize */
     (GClassInitFunc) xtk_cell_renderer_widget_class_init,
     0,    /* class_finalize */
     0,    /* class_data */
     sizeof(XtkCellRendererWidget),
     0,    /* n_preallocs */
     (GInstanceInitFunc) xtk_cell_renderer_widget_init,
   };
   return g_type_register_static(GTK_TYPE_CELL_RENDERER,
                                 "XtkCellRendererWidget",
                                 &renderer_widget_info, 0);
}
