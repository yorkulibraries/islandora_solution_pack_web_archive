<?php
/**
 * @file
 * This is the template file for the object page for web archives
 *
 * @TODO: add documentation about file and available variables
 */
?>

<div class="islandora-web-archive-object islandora">
  <div class="islandora-web-archive-content-wrapper clearfix">
    <?php if (isset($islandora_content)): ?>
      <div class="islandora-web-archive-content">
        <?php print $islandora_content; ?>
      </div>
    <?php endif; ?>
    </div>
    <div class="islandora-web-archive-metadata">
      <?php print $description; ?>
    <?php if ($parent_collections): ?>
      <div>
        <h2><?php print t('In collections'); ?></h2>
        <ul>
          <?php foreach ($parent_collections as $collection): ?>
            <li><?php print l($collection->label, "islandora/object/{$collection->id}"); ?></li>
          <?php endforeach; ?>
        </ul>
      </div>
    <?php endif; ?>
    <?php print $metadata; ?>
  </div>
</div>
