o = Object.new

loop do
	i = 1
	# o.myputs "index:" + i.to_s
	o.send_note(i, 127, 1)
	o.send_note(i, 0, 1)
	i = i + 1
end
