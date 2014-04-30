package ru.romanov.schedule.utils;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import javax.xml.parsers.ParserConfigurationException;
import java.util.Map;

public abstract class RequestStringsCreater {
	
	public static String CHECK_AGREE = "agree";
	public static String CHECK_DISAGREE = "disagree";
	
	
	private static String REQUEST_VERSION = "0.1";
	private static String REQUEST_TYPE_AUTH = "auth";
	private static String REQUEST_TYPE_UPDATE = "update";
	private static String REQUEST_TYPE_CHECK_UPDATES = "last_update_info";
	private static String REQUEST_TYPE_CONFIRM_CHECK = "check";
	
	//public static String MY_URI = "http://172.16.0.2:19090/main";
	/**
	 * Создать строку - тело запроса авторизации
	 * @param login
	 * @param pass
	 * @return
	 * @throws ParserConfigurationException 
	 */
	public static String createAuthRequestSting(String login, String pass) throws ParserConfigurationException {
		Document doc = XMLParser.createDocument();
		
        Element rootNode = doc.createElement("request");
        rootNode.setAttribute("version", REQUEST_VERSION);
        rootNode.setAttribute("type", REQUEST_TYPE_AUTH);
        doc.appendChild(rootNode);
        
        Element loginNode = doc.createElement("login");
        loginNode.setTextContent(login);
        Element passNode = doc.createElement("pass");
        passNode.setTextContent(pass);
        rootNode.appendChild(loginNode);
        rootNode.appendChild(passNode);
        
        return XMLParser.documentToString(doc);
	}
	/**
	 * Создать строку - тело запроса на загрузку изменений
	 * @param token
	 * @return
	 * @throws ParserConfigurationException 
	 */
	public static String createUpdateString (String token) throws ParserConfigurationException {
		Document doc = XMLParser.createDocument();
		Element rootNode = doc.createElement("request");
		rootNode.setAttribute("version", REQUEST_VERSION);
        rootNode.setAttribute("type", REQUEST_TYPE_UPDATE);
        doc.appendChild(rootNode);
        Element tokenNode = doc.createElement("token");
        tokenNode.setTextContent(token);
        rootNode.appendChild(tokenNode);
		return XMLParser.documentToString(doc);

	}
	
	/**
	 * Создать строку - тело запроса проверки наличия обновлений
	 * @param token
	 * @return
	 * @throws ParserConfigurationException 
	 */
	public static String createCheckUpdateString (String token) throws ParserConfigurationException {
		Document doc = XMLParser.createDocument();
		Element rootNode = doc.createElement("request");
        rootNode.setAttribute("version", REQUEST_VERSION);
        rootNode.setAttribute("type", REQUEST_TYPE_CHECK_UPDATES);
        doc.appendChild(rootNode);
        Element tokenNode = doc.createElement("token");
        tokenNode.setTextContent(token);
        rootNode.appendChild(tokenNode);
		return XMLParser.documentToString(doc);

	}
	
	/**
	 * Создать строку - тело запроса подтверждения получения изменений.
	 * @param token
	 * @param idMap - карта, формата <ID, CHECK_STATUS(CHECK_AGREE, CHECK_DISAGREE)>
	 * @return
	 * @throws ParserConfigurationException 
	 */
	public static String createConfirmCheckString (String token, Map <String, String> idMap) throws ParserConfigurationException {
		Document doc = XMLParser.createDocument();
		Element rootNode = doc.createElement("request");
        rootNode.setAttribute("version", REQUEST_VERSION);
        rootNode.setAttribute("type", REQUEST_TYPE_CONFIRM_CHECK);
        doc.appendChild(rootNode);
        Element tokenNode = doc.createElement("token");
        tokenNode.setTextContent(token);
        rootNode.appendChild(tokenNode);
        
        for (String mID: idMap.keySet()) {
        	Element id = doc.createElement("id");
        	id.setAttribute("checked", idMap.get(mID));
        	rootNode.appendChild(id);
        }
		return XMLParser.documentToString(doc);

	}
	
}
