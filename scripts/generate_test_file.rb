#
# inputs:
#    test_file_name.c
# outputs:
#   .tmp/filename.gt
#
require 'tempfile'
require 'fileutils'

created_files = []
test_file = File.open(ARGV[0], "r")
contents = test_file.read

puts File.basename(test_file, ".*")
puts File.dirname(test_file);

contents.scan(/--FILE\s+(.*?)--(.*?)--ENDFILE--/m).each do |matches|
  filename,content = matches
  content[0] = '' # remove first newline char
  content.chomp!
  if !created_files.include?(filename)
    File.open(".tmp/gt/#{filename}", "w") { |f| f.write(content) }
    created_files << filename
  end
end

temp_file = Tempfile.new("gtest")
begin
  contents.each_line do |line|
    start_create = line.match(/--FILE\s+(.*?)--/)
    end_create = line.match(/--ENDFILE--/)
    if start_create || end_create
      if start_create
        temp_file.puts("/*#{line}")
      else
        temp_file.puts("#{line}*/")
      end
    else
      temp_file.puts(line)
    end
  end
  temp_file.close
  FileUtils.mv(temp_file.path, "#{File.dirname(test_file)}/#{File.basename(test_file, ".*")}.c")
ensure
  temp_file.close
  temp_file.unlink
end