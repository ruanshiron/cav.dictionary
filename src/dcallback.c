#include <string.h>
#include <gtk/gtk.h>

#include "bt-5.0.0/inc/btree.h"
#include "dcallback.h"

int isBlank(char * text)
{
    int i=0, l=strlen(text);
    while (i<l)
    {
        if (!((text[i]==' ') || (text[i]=='\n'))) 
        {  
            return 0;
        }
        else i++;
    }
    return 1;
}

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

    //GtkTextBuffer *buffer;
    //buffer = gtk_text_buffer_new (NULL);

    if (!rsize) 
    {
        //"Khong tim thay"
        gtk_label_set_label(GTK_LABEL(word_label), "");
        //Cap Nhat Buffer
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), "", -1);
        //Set Buffer moi vao Text View
        gtk_text_view_set_buffer(GTK_TEXT_VIEW (meaning_textview), GTK_TEXT_BUFFER( textbuffer));
        
        //Notify Popup 
        gtk_label_set_label (GTK_LABEL(pop_label), "Không tìm thấy");
        gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(search_entry)) ;
        gtk_popover_popup (GTK_POPOVER(popover));

        //An Update Button  "Sửa"
        gtk_widget_set_sensitive (GTK_WIDGET(update_button), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(add_button), TRUE);

    }
    else 
    {
        //Tim thay Word
        gtk_label_set_label(GTK_LABEL(word_label), word);
        //New Meaning >> Buffer
        gtk_text_buffer_set_text(GTK_TEXT_BUFFER(textbuffer), meaning, -1);
        //
        gtk_text_view_set_buffer(GTK_TEXT_VIEW (meaning_textview) ,GTK_TEXT_BUFFER(textbuffer));

        //Update Button: "Them"
        gtk_widget_set_sensitive (GTK_WIDGET(update_button), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(add_button), FALSE);

        
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
    char * word = gtk_entry_get_text (GTK_ENTRY(search_entry));

    gtk_label_set_label (GTK_LABEL(word_label), word);

    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), TRUE);

    gtk_label_set_label (GTK_LABEL(notify_label), "Do you want to Add this word?");
    gtk_widget_set_visible (GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_add), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_update), FALSE);
}

void update_button_clicked (GtkButton * button, gpointer NONE)
{
    char * word = gtk_entry_get_text (GTK_ENTRY(search_entry));

    gtk_label_set_label (GTK_LABEL(word_label), word);

    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), TRUE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), TRUE);
    
    gtk_label_set_label (GTK_LABEL(notify_label), "Do you want to Update this word?");
    gtk_widget_set_visible (GTK_WIDGET(info_bar), TRUE);
    gtk_widget_set_visible (GTK_WIDGET(yes_delete), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_add), FALSE);
    gtk_widget_set_visible (GTK_WIDGET(yes_update), TRUE);
}

void no_button_clicked (GtkButton * button, gpointer NONE)
{
    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), FALSE);

    gtk_widget_set_visible (GTK_WIDGET(info_bar), FALSE);
}

void yes_delete_clicked (GtkButton * button, gpointer NONE)
{
    char * word = gtk_label_get_text (GTK_LABEL(word_label));
    int fail = btdel (data, word);
    char notify[255];
    if (!fail) strcpy (notify, "Đã xóa \"");
    else strcpy (notify, "Không thể xóa  \"");
    strcat (notify, word);
    strcat (notify, "\"  khỏi từ điển!");

    gtk_label_set_label (GTK_LABEL(pop_label), notify);
    gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(delete_button)) ;
    gtk_popover_popup (GTK_POPOVER(popover));

    gtk_widget_set_visible (GTK_WIDGET(info_bar), FALSE);
}

void yes_add_clicked (GtkButton * button, gpointer NONE)
{
    char * word = gtk_label_get_text (GTK_LABEL(word_label));

    GtkTextIter startIter;
	GtkTextIter endIter;

    gtk_text_buffer_get_start_iter (textbuffer, &startIter);
    gtk_text_buffer_get_end_iter (textbuffer, &endIter);
    
    char * meaning = gtk_text_buffer_get_text (GTK_TEXT_BUFFER(textbuffer), &startIter, &endIter, FALSE);
    if (isBlank(meaning)) 
    {
        gtk_label_set_label (GTK_LABEL(pop_label), "Không được để trống");
        gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(add_button)) ;
        gtk_popover_popup (GTK_POPOVER(popover));
        return;
    }

    int fail = btins (data, word, meaning, strlen(meaning)+1);

    char notify[255];
    if (!fail) strcpy (notify, "Đã thêm \"");
    else strcpy (notify, "Không thể thêm  \"");
    strcat (notify, word);
    strcat (notify, "\"  vào từ điển!");

    gtk_label_set_label (GTK_LABEL(pop_label), notify);
    gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(add_button)) ;
    gtk_popover_popup (GTK_POPOVER(popover));

    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), FALSE);

    gtk_widget_set_visible (GTK_WIDGET(info_bar), FALSE);
}

void yes_update_clicked (GtkButton * button, gpointer NONE)
{
    char * word = gtk_label_get_text (GTK_LABEL(word_label));

    GtkTextIter startIter;
	GtkTextIter endIter;

    gtk_text_buffer_get_start_iter (textbuffer, &startIter);
    gtk_text_buffer_get_end_iter (textbuffer, &endIter);

    char * meaning = gtk_text_buffer_get_text (GTK_TEXT_BUFFER(textbuffer), &startIter, &endIter, FALSE);
    if (isBlank(meaning)) 
    {
        gtk_label_set_label (GTK_LABEL(pop_label), "Không được để trống");
        gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(add_button)) ;
        gtk_popover_popup (GTK_POPOVER(popover));
        return;
    }

    int fail = btupd (data, word, meaning, strlen(meaning)+1);

    char notify[255];
    if (!fail) strcpy (notify, "Đã cập nhật \"");
    else strcpy (notify, "Không thể cập nhật  \"");
    strcat (notify, word);
    strcat (notify, "\"  vào từ điển!");

    gtk_label_set_label (GTK_LABEL(pop_label), notify);
    gtk_popover_set_relative_to (GTK_POPOVER(popover), GTK_WIDGET(update_button)) ;
    gtk_popover_popup (GTK_POPOVER(popover));

    gtk_text_view_set_editable (GTK_TEXT_VIEW (meaning_textview), FALSE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (meaning_textview), FALSE);

    gtk_widget_set_visible (GTK_WIDGET(info_bar), FALSE);
}