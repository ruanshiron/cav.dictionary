#include <string.h>
#include <gtk/gtk.h>

#include "bt-5.0.0/inc/btree.h"
#include "dcallback.h"


void separate_mean(char* mean) 
{
    int i = 0, j = 1;
    while (mean[j] != '\0') 
    {
        if (mean[j] == '\\' && mean[j + 1] == 'n') 
        {
            mean[i++] = '\n';
            j += 2;
        }
        else {
            if (i != j)
                mean[i++] = mean[j++];
            else 
            {
                i++; j++;
            }
        }
    }
    mean[i] = '\0';
}

int convert_text_to_bt(char * filename)
{
    FILE * datafile;
    if ((datafile = fopen(filename, "r")) == NULL) 
    {
        printf("Loi mo file\n");
        return 0;;
    };

    BTA *convertfile;
    convertfile = btcrt("resource.dat", 0, 0);

    char word[WORD_LENGHT], mean[MEANING_LENGHT];
    int line;

    while (fscanf(datafile, "%[^\t]", word) == 1) 
    {
        fgets(mean, MEANING_LENGHT, datafile);
        line++;
        separate_mean(mean);
        btins(convertfile, word, mean, strlen(mean) + 1);    
    }

    printf("%d lines.\n",line);
    btcls(convertfile);
    fclose(datafile);
    return 1;
}

void search_entry_activate (GtkEntry * entry, gpointer NONE)
{
    char word[WORD_LENGHT];
    char meaning[MEANING_LENGHT];
    strcpy (word, gtk_entry_get_text (entry));

    int rsize = 0;
    btsel (data, word, meaning, MEANING_LENGHT, &rsize);

    GtkTextBuffer *buffer;
    buffer = gtk_text_buffer_new (NULL);

    if (!rsize) 
    {
        //"Khong tim thay"
        gtk_label_set_label(GTK_LABEL(word_label), "Không tìm thấy");
        //Cap Nhat Buffer
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), "404\0", -1);
        //Set Buffer moi vao Text View
        gtk_text_view_set_buffer(GTK_TEXT_VIEW (meaning_textview), GTK_TEXT_BUFFER( buffer));

        //An Update Button  "Sửa"
        gtk_widget_set_visible (GTK_WIDGET(update_button), FALSE);
        gtk_widget_set_visible (GTK_WIDGET(add_button), TRUE);

    }
    else 
    {
        //Tim thay Word
        gtk_label_set_label(GTK_LABEL(word_label), word);
        //New Meaning >> Buffer
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), meaning, -1);
        //
        gtk_text_view_set_buffer(GTK_TEXT_VIEW (meaning_textview) ,GTK_TEXT_BUFFER(buffer));

        //Update Button: "Them"
        gtk_widget_set_visible (GTK_WIDGET(update_button), TRUE);
        gtk_widget_set_visible (GTK_WIDGET(add_button), FALSE);

        
    }
}

void delete_button_clicked (GtkButton * button, gpointer NONE)
{
    gtk_label_set_label (GTK_LABEL(notify_label), "Do you want to Delete this word?");
    gtk_widget_set_visible (GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_delete), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_add), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_update), FALSE);
}

void add_button_clicked (GtkButton * button, gpointer NONE)
{
    gtk_label_set_label (GTK_LABEL(notify_label), "Do you want to Add this word?");
    gtk_widget_set_visible (GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_add), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_update), FALSE);
}

void update_button_clicked (GtkButton * button, gpointer NONE)
{
    gtk_label_set_label (GTK_LABEL(notify_label), "Do you want to Update this word?");
    gtk_widget_set_visible (GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_add), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_update), TRUE);
}

void no_button_clicked (GtkButton * button, gpointer NONE)
{
    gtk_widget_set_visible (GTK_WIDGET(info_bar), FALSE);
}

