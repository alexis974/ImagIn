#include <gexiv2/gexiv2.h>
#include <gexiv2/gexiv2-metadata.h>

#include "../debug/error_handler.h"
#include "../tools/strings.h"

#include "../gui/gui.h"

void print_data(const char *filename)
{
    if(!gexiv2_initialize())
    {
        throw_error("print_data:", "Could not init gexiv");
        return;
    }
    GExiv2Metadata *meta_data = gexiv2_metadata_new();
    if(!meta_data)
    {
        throw_error("print_data:", "Could not init gexiv");
        return;
    }
    if(!gexiv2_metadata_open_path(meta_data, filename, NULL))
    {
        throw_error("print_data:", "Could not read metadata");
        g_object_unref (meta_data);
        return;
    }
    if(!gexiv2_metadata_get_supports_exif (meta_data))
    {
        printf("Image not supporting exif format.");
        return;
    }
    printf("--- IMAGE INFO ---\n");
    printf("Exposure time is %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ExposureTime"));
    printf("Shutter speed is %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ShutterSpeedValue"));
    printf("ISO is %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ISOSpeedRatings"));
    printf("Focal Length is %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.FocalLength"));
    printf("Photo taken on the %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.DateTimeOriginal"));
    printf("Aperture is %s\n",gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ApertureValue"));
    printf("-----------------\n");
}

void set_image_info(const char *filename, struct UI *ui)
{
    const char *file_name = get_filename_from_path(filename);
    gtk_label_set_text(ui->image_info->filename, file_name);
    if(!gexiv2_initialize())
    {
        throw_error("print_data:", "Could not init gexiv");
        return;
    }
    GExiv2Metadata *meta_data = gexiv2_metadata_new();
    if(!meta_data)
    {
        throw_error("print_data:", "Could not init gexiv");
        return;
    }
    if(!gexiv2_metadata_open_path(meta_data, filename, NULL))
    {
        throw_error("print_data:", "Could not read metadata");
        g_object_unref (meta_data);
        return;
    }
    if(!gexiv2_metadata_get_supports_exif (meta_data))
    {
        printf("Image not supporting exif format.");
        return;
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.ExposureTime"))
    {
        gtk_label_set_text(ui->image_info->exposure_time,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ExposureTime"));
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.ShutterSpeedValue"))
    {
        gtk_label_set_text(ui->image_info->shutter_speed,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ShutterSpeedValue"));
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.ISOSpeedRatings"))
    {
        gtk_label_set_text(ui->image_info->iso,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ISOSpeedRatings"));
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.FocalLength"))
    {
        gtk_label_set_text(ui->image_info->focal_length,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.FocalLength"));
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.ApertureValue"))
    {
        gtk_label_set_text(ui->image_info->aperture,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.ApertureValue"));
    }
    if(gexiv2_metadata_get_tag_interpreted_string(meta_data, "Exif.Photo.DateTimeOriginal"))
    {
        gtk_label_set_text(ui->image_info->date,
            gexiv2_metadata_get_tag_interpreted_string (meta_data, "Exif.Photo.DateTimeOriginal"));
    }
    gtk_label_set_text(ui->image_info->time, "-");
}