<?
auth = {}
auth.valid = false
auth.err = ""
auth.user = nil

local function check_session()
	local client = redis.connect(conf.redis_params)
	client:select(15) -- for testing purposes

	local user = client:get('session_'..env.session)
	if user then
		local ses = client:get(user..'_session')
		if ses == env.session then
			auth.valid = true
			auth.user = user
		else
			auth.err = "User has logined with another session"
		end
	else
		auth.err = "Session is not exist"
	end
end

local function bind_session(user)
	local client = redis.connect(conf.redis_params)
	client:select(15) -- for testing purposes

	local org_session = client:get(user..'_session')
	if org_session then
		client:del('session_'..org_session)
	end
	client:set(user..'_session', env.session)
	client:set('session_'..env.session, user)
end

auth.check_user_valid = function (user, pass) 
	if not user or not pass then
		return false, "no user or password input"
	end
	local client = redis.connect(conf.redis_params)
	client:select(15) -- for testing purposes
	local pwd = client:get(user..'_pw')

	if pwd == pass then
		bind_session(user)
		return true
	else
		return false, "password incorrect!!"
	end
end

auth.logout = function (user)
	local client = redis.connect(conf.redis_params)
	client:select(15) -- for testing purposes

	local org_session = client:get(user..'_session')
	if org_session then
		client:del('session_'..org_session)
	end
	client:del(user..'_session')
end
	
check_session()
?>

