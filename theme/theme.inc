<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */
function islandora_web_archive_preprocess_islandora_web_archive(array &$variables) {
  module_load_include('inc', 'islandora', 'includes/datastream');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  module_load_include('inc', 'islandora', 'includes/authtokens');
  module_load_include('inc', 'islandora', 'includes/metadata');
  drupal_add_js('misc/form.js');
  drupal_add_js('misc/collapse.js');
  $object = $variables['object'];

  // We should eventually remove the DC object and dc_array code as it only
  // exists to not break legacy implementations.
  try {
    if (isset($object['DC']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['DC'])) {
      $dc = $object['DC']->content;
      $dc_object = DublinCore::importFromXMLString($dc);
    }
  }
  catch (Exception $e) {
    drupal_set_message(t('Error retrieving object %s %t', array('%s' => $object->id, '%t' => $e->getMessage())), 'error', FALSE);
  }
  $variables['islandora_dublin_core'] = isset($dc_object) ? $dc_object : NULL;
  $variables['dc_array'] = isset($dc_object) ? $dc_object->asArray() : array();
  $variables['islandora_object_label'] = $object->label;
  $variables['theme_hook_suggestions'][] = 'islandora_web_archive__' . str_replace(':', '_', $object->id);
  $variables['parent_collections'] = islandora_get_parents_from_rels_ext($object);
  $variables['metadata'] = islandora_retrieve_metadata_markup($object);
  $variables['description'] = islandora_retrieve_description_markup($object);

  // Original.
  if (isset($object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OBJ'])) {
    $warc_url = url("islandora/object/{$object->id}/datastream/OBJ/view", array('absolute' => TRUE));
    $variables['islandora_warc'] = $warc_url;
  }

  // Thumbnail.
  if (isset($object['TN']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['TN'])) {
    $thumbnail_size_url = url("islandora/object/{$object->id}/datastream/TN/view");
    $params = array(
      'title' => $object->label,
      'path' => $thumbnail_size_url,
    );
    $variables['islandora_thumbnail_img'] = theme('image', $params);
  }

  // Download warc.
  module_load_include('inc', 'islandora', 'includes/mimetype.utils');
  if (isset($object['OBJ']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['OBJ'])) {
    $file_ext = islandora_get_extension_for_mimetype($object['OBJ']->mimetype);
    $warc_url = url("islandora/object/{$object->id}/datastream/OBJ/{$object->label}.{$file_ext}", array('absolute' => TRUE));
    $variables['islandora_warc'] = l($variables['islandora_object_label'] . '.' . $file_ext, $warc_url, array('html' => TRUE));
  }

  // Download pdf.
  if (isset($object['PDF']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['PDF'])) {
    $pdf_url = url("islandora/object/{$object->id}/datastream/PDF/{$object->label}.pdf", array('absolute' => TRUE));
    $variables['islandora_pdf'] = l($variables['islandora_object_label'] . '.pdf', $pdf_url, array('html' => TRUE));
  }

  // Download screenshot.
  if (isset($object['SCREENSHOT']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['SCREENSHOT'])) {
    $file_ext = islandora_get_extension_for_mimetype($object['SCREENSHOT']->mimetype);
    $file_url = url("islandora/object/{$object->id}/datastream/SCREENSHOT/{$object->label}.{$file_ext}", array('absolute' => TRUE));
    $variables['islandora_screenshot'] = l($variables['islandora_object_label'] . '.' . $file_ext, $file_url, array('html' => TRUE));
  }

  // Download CSV.
  if (isset($object['WARC_CSV']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['WARC_CSV'])) {
    $csv_url = url("islandora/object/{$object->id}/datastream/WARC_CSV/{$object->label}.csv", array('absolute' => TRUE));
    $variables['islandora_csv'] = l($variables['islandora_object_label'] . '.csv', $csv_url, array('html' => TRUE));
  }

  // Download CDX.
  if (isset($object['CDX']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['CDX'])) {
    $cdx_url = url("islandora/object/{$object->id}/datastream/CDX/{$object->label}.cdx", array('absolute' => TRUE));
    $variables['islandora_cdx'] = l($variables['islandora_object_label'] . '.cdx', $cdx_url, array('html' => TRUE));
  }

  // Download WAT.
  if (isset($object['WAT']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['WAT'])) {
    $wat_url = url("islandora/object/{$object->id}/datastream/WAT/{$object->label}.wat.gz", array('absolute' => TRUE));
    $variables['islandora_wat'] = l($variables['islandora_object_label'] . '.wat.gz', $wat_url, array('html' => TRUE));
  }

  // Download SEEDLIST.
  if (isset($object['SEEDLIST']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['SEEDLIST'])) {
    $seedlist_url = url("islandora/object/{$object->id}/datastream/SEEDLIST/{$object->label}.txt", array('absolute' => TRUE));
    $variables['islandora_seedlist'] = l($variables['islandora_object_label'] . '.txt', $seedlist_url, array('html' => TRUE));
  }

  // Download HERITRIX.
  if (isset($object['HERITRIX']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['HERITRIX'])) {
    $heritrix_url = url("islandora/object/{$object->id}/datastream/HERITRIX/{$object->label}.txt", array('absolute' => TRUE));
    $variables['islandora_heritrix'] = l('crawler-beans.cxml', $heritrix_url, array('html' => TRUE));
  }

  // Medium size.
  if (isset($object['JPG']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['JPG'])) {
    $medium_size_url = url("islandora/object/{$object->id}/datastream/JPG/view");
    $params = array(
      'title' => $object->label,
      'path' => $medium_size_url,
    );
    $variables['islandora_medium_img'] = theme('image', $params);
    if (isset($medium_size_url)) {
      $variables['islandora_content'] = l($variables['islandora_medium_img'], $medium_size_url, array('html' => TRUE));
    }
    else {
      $variables['islandora_content'] = $variables['islandora_medium_img'];
    }
  }
}
