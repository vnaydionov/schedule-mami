package ru.romanov.schedule.adapters;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import ru.romanov.schedule.utils.StringConstants;

import java.util.HashMap;
import java.util.Map;

public class UserAdapter {
	public static final String NAME = "name";
	public static final String LOGIN = "login";
	public static final String EMAIL = "email";
	public static final String PHONE = "phone";
	
	private Context context;
	
	public UserAdapter(Context context) {
		this.context = context;
	}
	

	public Map<String, String> getUserAsMap() {
		Map<String, String> user = new HashMap<String, String>();
		SharedPreferences mSharedPreferences = context.getSharedPreferences(
				StringConstants.SCHEDULE_SHARED_PREFERENCES, Context.MODE_PRIVATE);
		user.put("name", mSharedPreferences.getString("name", "-"));
		user.put("login", mSharedPreferences.getString("login", "-"));
		user.put("phone", mSharedPreferences.getString("phone", "-"));
		user.put("email", mSharedPreferences.getString("email", "-"));
		return user;
	}
	
	public void saveUser(String name, String login, String email, String phone) {
		SharedPreferences mSharedPreferences = context.getSharedPreferences(
				StringConstants.SCHEDULE_SHARED_PREFERENCES, Context.MODE_PRIVATE);
		Editor editor = mSharedPreferences.edit();
		editor.putString("name", name);
		editor.putString("login", login);
		editor.putString("email", email);
		editor.putString("phone", phone);
		editor.commit();
	}
}
