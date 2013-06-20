class HelloMRubyPlugin < SylpheedPlugin

  def initialize
    @plugin_info = {
      'author' => 'HAYASHI Kentaro',
      'version' => '0.1.0',
      'name' => 'SylMRuby',
      'description' => 'Base SylMRuby plugin as a sample plugin'
    }
  end

  def init_done
    p "Hello, called from sylmruby plugin"
  end
end