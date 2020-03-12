<?php

/**
 * @file
 * Definition of views_handler_field_node_link.
 */

/**
 * Field handler to present a link to the node.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_node_link extends views_handler_field_entity {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby.
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    if (module_exists('translation')) {
      $this->additional_fields['language'] = array('table' => 'node', 'field' => 'language');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->render_link($entity, $values);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function render_link($node, $values) {
    if (node_access('view', $node)) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "node/$node->nid";
      $this->render_link_options($node, $values);
      $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
      return $text;
    }
  }

  /**
   * Adds some additional options to the link to be rendered.
   */
  protected function render_link_options($node, $values) {
    if (isset($this->aliases['language'])) {
      $languages = language_list();
      $language = $this->get_value($values, 'language');
      if (isset($languages[$language])) {
        $this->options['alter']['language'] = $languages[$language];
      }
      else {
        unset($this->options['alter']['language']);
      }
    }
  }
}
