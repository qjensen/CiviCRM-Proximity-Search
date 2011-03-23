<?php 

class civicrm_proximity_handler_filter_circle extends civicrm_proximity_handler_filter {
 
  /**
   * Process operator.
   */
  function op_process($op = 'simple', $field) {
    parent::op_process($op, $field);
    
    if (!is_null($this->value['lat']) && !is_null($this->value['lon'])) {
          
      $this->query->add_field(NULL, $this->table_formula($this->value['lat'], $this->value['lon']), 'distance');
      
      if ($op == 'simple') {
        $radius = $this->value['value'];
        $where = ($this->operator == '<') ? $this->build_where('<', $radius) : $this->build_where('>', $radius);
      }
      else {
        $inner = $this->build_where('>', $this->value['min']);
        $outer = $this->build_where('<', $this->value['max']);
        $where = ($this->operator == 'between') ? "($inner AND $outer)" : "NOT($inner AND $outer)";
      }
      $this->query->add_having($this->options['group'], $where);
    }
  }

  /**
   * Build WHERE clause.
   */
  function build_where($operator, $radius) {
    return sprintf("(distance $operator= %f)", $radius);
  }

  function table_formula($lat, $lon) {
    $args = array('!lat1' => 'geo_code_1', '!lon1' => 'geo_code_2', '!lat2' => $lat, '!lon2' => $lon);
    $great_circle = strtr(CIVICRM_PROXIMITY_SQL_GREAT_CIRCLE, $args);
    return "($great_circle)";
  }
}
