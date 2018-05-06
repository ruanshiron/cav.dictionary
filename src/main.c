#include <gtk/gtk.h>
#include "bt-5.0.0/inc/btree.h"

//Global Object

//Global DATA
BTA * data = NULL;

//Global variable

//testing callback function
static void hello_word (GtkWidget * widget, gpointer data)
{
    g_print ("hola\n");
} 

int main (int argc, char *argv[])
{
    //Khoi tao data
    data = btopn ("resource.dat", 0, 1);

    //Khai bao cac doi tuong >> Local
    GtkBuilder *builder;
    GObject *window, 
            *info_button, *delete_button, *update_button, *search_entry,
            *notify_label, *yes_button, *no_button,
            *word_label, *meaning_textview, 
            //No Shape Objects
            *textbuffer;

    //Khoi tao giao dien nguoi dung
    gtk_init (&argc, &argv);

    //Khoi tao GtkBuilder
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "glade/newW.ui", NULL);

    //Khoi tao Window >> Tin hieu "tat cua so"
    window = gtk_builder_get_object (builder, "window");
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    /* TODO Khoi tao tin hieu cua cac Object khac o day*/

        //Khoi tao Info Button
        info_button = gtk_builder_get_object (builder, "info_button");
        g_signal_connect (info_button, "clicked", G_CALLBACK (hello_word), NULL);

        //Khoi tao Delete Button
        delete_button = gtk_builder_get_object (builder, "delete_button");
        g_signal_connect (delete_button, "clicked", G_CALLBACK (hello_word), NULL);

        //Khoi tao Update Button
        update_button = gtk_builder_get_object (builder, "update_button");
        g_signal_connect (update_button, "clicked", G_CALLBACK (hello_word), NULL);

        
    //Hien thi toan bo Window va "Child"
    gtk_widget_show_all (window); 

    //Ngung tham chieu Builder (free)
    g_object_unref (builder);

    //Chay vong lap main (the main loop) cho den khi gtk_main_quit() duoc goi
    gtk_main ();

    //Ngung tham chieu DATA >> Tat chuong trinh
    btcls (data);

    return 0;
}