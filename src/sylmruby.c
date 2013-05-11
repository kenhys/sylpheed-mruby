/*
 * SylMRuby -- 
 * Copyright (C) 2013 HAYASHI Kentaro
 *
 */

#include "config.h"
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libintl.h>

#include <mruby.h>
#include "mruby/array.h"
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#ifdef ENABLE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include <mruby/string.h>

#include "sylmain.h"
#include "plugin.h"
#include "folder.h"
#include "procmsg.h"
#include "compose.h"
#include "alertpanel.h"
#include "foldersel.h"
#include "headerview.h"
#include "messageview.h"
#include "procheader.h"
#include "sylplugin_factory.h"
#include "sylmruby.h"
#include "copying.h"

#undef PLUGIN_NAME
#define PLUGIN_NAME N_("SylMRuby Plug-in")

#undef PLUGIN_DESC
#define PLUGIN_DESC N_("Support mruby interface for Sylpheed")

static SylPluginInfo info = {
  N_(PLUGIN_NAME),
  VERSION,
  "HAYASHI Kentaro",
  N_(PLUGIN_DESC)
};

SylMRubyOption SYLPF_OPTION;

static mrbc_context *ctx;
static struct mrb_parser_state *parser;
static mrb_state *mrb;
static mrb_value sylmruby;

gulong app_exit_handler_id = 0;

void plugin_load(void)
{
  gpointer data;

  SYLPF_START_FUNC;

  syl_init_gettext(SYLMRUBY, "lib/locale");

  syl_plugin_add_menuitem("/Tools", NULL, NULL, NULL);
  syl_plugin_add_menuitem("/Tools",
                          _("SylMRuby [sylmruby]"),
                          exec_sylmruby_menu_cb, NULL);

  g_signal_connect_after(syl_app_get(),
                           "init-done",
                           G_CALLBACK(init_done_cb),
                           NULL);
  app_exit_handler_id = g_signal_connect(syl_app_get(),
                                           "app-exit",
                                           G_CALLBACK(app_exit_cb),
                                           NULL);
  syl_plugin_signal_connect("folderview-menu-popup",
                              G_CALLBACK(folderview_menu_popup_cb), NULL);
  syl_plugin_signal_connect("summaryview-menu-popup",
                              G_CALLBACK(summaryview_menu_popup_cb), NULL);
  syl_plugin_signal_connect("textview-menu-popup",
                              G_CALLBACK(textview_menu_popup_cb), NULL);
  syl_plugin_signal_connect("compose-created",
                              G_CALLBACK(compose_created_cb), NULL);
  syl_plugin_signal_connect("compose-destroy",
                              G_CALLBACK(compose_destroy_cb), NULL);
  syl_plugin_signal_connect("compose-send",
                              G_CALLBACK(compose_send_cb), NULL);
  syl_plugin_signal_connect("messageview-show",
                              G_CALLBACK(messageview_show_cb), NULL);
  syl_plugin_signal_connect("inc-mail-start",
                              G_CALLBACK(inc_start_cb), NULL);
  syl_plugin_signal_connect("inc-mail-finished",
                              G_CALLBACK(inc_finished_cb), NULL);

  data = syl_plugin_main_window_get();
  syl_plugin_main_window_popup(data);

  data = &SYLPF_OPTION;
  sylpf_load_option_rcfile((SylPluginFactoryOption*)data,
                           SYLMRUBYRC);

  init_mruby();

  SYLPF_END_FUNC;
}

void plugin_unload(void)
{
  SYLPF_START_FUNC;
  SYLPF_END_FUNC;
}

SylPluginInfo *plugin_info(void)
{
  return &info;
}

gint plugin_interface_version(void)
{
  /* return SYL_PLUGIN_INTERFACE_VERSION; */
  /* Sylpheed 3.3.0Beta1 or later */
  return 0x0109;
}

static void save_sylmruby_preference(SylMRubyOption *option)
{
  SYLPF_START_FUNC;

#define TOGGLE_STATE(widget) \
  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))

#undef TOGGLE_STATE

  SYLPF_END_FUNC;
}

static void exec_sylmruby_menu_cb(void)
{
  GtkWidget *dialog;
  gint response;

  SYLPF_START_FUNC;

  dialog = create_preference_dialog(&SYLPF_OPTION);

  gtk_widget_show_all(dialog);
  response = gtk_dialog_run(GTK_DIALOG(dialog));

  switch (response) {
  case GTK_RESPONSE_OK:
    save_sylmruby_preference(&SYLPF_OPTION);
    break;
  case GTK_RESPONSE_CANCEL:
  default:
    break;
  }

  gtk_widget_destroy(dialog);

  SYLPF_END_FUNC;
}

static GtkWidget *create_preference_dialog(SylMRubyOption *option)
{
  GtkWidget *vbox, *hbox;
  GtkWidget *dialog;
  gpointer mainwin;
  GtkWidget *window;
  GtkWidget *notebook;

  SYLPF_START_FUNC;

  mainwin = syl_plugin_main_window_get();
  window = ((MainWindow*)mainwin)->window;

  dialog = gtk_dialog_new_with_buttons(_("SylMRuby"),
                                       GTK_WINDOW(window),
                                       GTK_DIALOG_MODAL,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OK, GTK_RESPONSE_OK,
                                       NULL);

  sylpf_init_preference_dialog_size(dialog);

  vbox = gtk_vbox_new(FALSE, SYLPF_BOX_SPACE);
  hbox = gtk_hbox_new(TRUE, SYLPF_BOX_SPACE);

  gtk_container_add(GTK_CONTAINER(hbox), vbox);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);

  notebook = gtk_notebook_new();
  create_config_main_page(notebook, SYLPF_OPTION.rcfile);
  sylpf_append_config_about_page(notebook,
                                 option->rcfile,
                                 _("About"),
                                 _(PLUGIN_NAME),
                                 _(PLUGIN_DESC),
                                 _(copyright));

  gtk_widget_show(notebook);
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), SYLPF_BOX_SPACE);

  SYLPF_RETURN_VALUE(dialog);
}

static GtkWidget *create_config_main_page(GtkWidget *notebook, GKeyFile *pkey)
{
  GtkWidget *vbox;
  GtkWidget *label;

  SYLPF_START_FUNC;

  vbox = gtk_vbox_new(FALSE, 6);

  label = gtk_label_new(_("General"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
  gtk_widget_show_all(notebook);

  SYLPF_RETURN_VALUE(vbox);
}

static void init_mruby(void)
{
  gchar *contents;
  gchar *rb_file;
  GError *error;
  gsize n_length;
  int n;
  int ai;
  mrb_value result;
  struct RClass *klass;
  mrb_value instance;

  mrb = mrb_open();
  ctx = mrbc_context_new(mrb);
  ctx->capture_errors = 1;
  ai = mrb_gc_arena_save(mrb);

  rb_file = g_build_filename(G_DIR_SEPARATOR_S,
                             get_rc_dir(),
                             "sylmruby.rb",
                             NULL);

  g_file_get_contents(rb_file, &contents, &n_length, &error);

  parser = mrb_parse_string(mrb, contents, ctx);

  n = mrb_generate_code(mrb, parser);

  result = mrb_run(mrb,
                   mrb_proc_new(mrb, mrb->irep[n]),
                   mrb_top_self(mrb));
  if (mrb->exc) {
    mrb->exc = 0;
  }

  klass = mrb_class_get(mrb, "SylpheedPlugin");
  instance = mrb_obj_value(klass);

  sylmruby = mrb_funcall(mrb, instance, "new", 0);
}

static void init_done_cb(GObject *obj, gpointer data)
{
  mrb_value result;

  result = mrb_funcall(mrb, sylmruby, "init_done", 0);
}

static void app_exit_cb(GObject *obj, gpointer data)
{
}


static void folderview_menu_popup_cb(GObject *obj, GtkItemFactory *ifactory,
                                     gpointer data)
{
}

static void summaryview_menu_popup_cb(GObject *obj, GtkItemFactory *ifactory,
                                      gpointer data)
{
}

static void textview_menu_popup_cb(GObject *obj, GtkMenu *menu,
                                   GtkTextView *textview,
                                   const gchar *uri,
                                   const gchar *selected_text,
                                   MsgInfo *msginfo)
{
}

static void compose_created_cb(GObject *obj, gpointer compose)
{
}

static void compose_destroy_cb(GObject *obj, gpointer compose)
{
}

static gboolean compose_send_cb(GObject *obj, gpointer compose,
                                gint compose_mode, gint send_mode,
                                const gchar *msg_file, GSList *to_list)
{
  return FALSE;
}

static void messageview_show_cb(GObject *obj, gpointer msgview,
                                MsgInfo *msginfo, gboolean all_headers)
{
  MessageView *messageview = NULL;

  SYLPF_START_FUNC;

  g_return_if_fail(msgview != NULL);

  messageview = (MessageView*)msgview;

  g_return_if_fail(messageview != NULL);

  SYLPF_END_FUNC;
}

static void inc_start_cb(GObject *obj, PrefsAccount *ac)
{
}

static void inc_finished_cb(GObject *obj, gint new_messages)
{
}


