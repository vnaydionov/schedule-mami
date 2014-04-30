package ru.romanov.schedule.utils;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.xpath.*;
import java.io.*;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Map;

public abstract class XMLParser {
	private static long MILISEC_IN_DAY = 1000 * 60 * 60 * 24;
	private static long MILISEC_IN_WEEK = MILISEC_IN_DAY * 7;

	public static String OK = "OK";
	public static String NO_AUTH = "NO_AUTH";
	public static String STATUS = "status";
	public static String TOKEN = "token";
	public static String ITEM = "item";
	public static String ID = "id";
	public static String UPDATE_DT = "update-dt";
	public static String MODE = "mode";
	public static String CHECKED = "checked";
	public static String MODE_ADD = "add";
	public static String MODE_DEL = "del";
	public static String PLACE = "place";
	public static String SUBJECT = "subject";
	public static String ACTS = "activities";
	public static String GROUPS = "groups";
	public static String TIME = "time";
	public static String PERIOD = "period";
	public static String START = "start";
	public static String END = "end";
	public static String REPEAT = "repeat";
	public static String DAY_OF_WEEK = "dow";
	public static String HOURS = "hours";
	public static String EXCLUDES = "excludes";
	public static String INCLUDES = "includes";
	public static String NAME = "name";
	public static String EMAIL ="email";
	public static String PHONE = "phone";
	public static String LOGIN = "login";
	public static String LAST_UPDATE_DT = "last_update_dt";
	public static String REPEAT_MODE_EACH = "each";
	public static String REPEAT_MODE_NONE = "none";
	public static String SHORT_STRING_DATE_FORMAT = "yyyy-MM-dd";
	public static String TIME_DATE_FORMAT = "kkmm";
	public static String MY_TIME_DATE_FORMAT = "kk:mm";
	public static String MY_LONG_DATE_FORMAT = "dd.MM.dd  kk:mm";

	/**
	 * —А–∞–Ј–±–Њ—А –Њ—В–≤–µ—В–∞ –љ–∞ –Ј–∞–њ—А–Њ—Б –∞–≤—В–Њ—А–Є–Ј–∞—Ж–Є–Є
	 * 
	 * @param XMLResponse
	 * @return
	 * @throws ParserConfigurationException
	 * @throws SAXException
	 * @throws IOException
	 */
	public static Document createDocument() throws ParserConfigurationException {
		DocumentBuilder builder = getBuilder();
		return builder.newDocument();
	}
	
	public static DocumentBuilder getBuilder() throws ParserConfigurationException {
		DocumentBuilderFactory factory = DocumentBuilderFactory
				.newInstance();
		return factory.newDocumentBuilder();
	}
	
	public static XPathExpression getXPathExpression(String str) {
		XPathExpression expr = null;
		XPathFactory factory = XPathFactory.newInstance();
	    XPath xpath = factory.newXPath();
	    try {
			expr = xpath.compile(str);
		} catch (XPathExpressionException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    return expr;
	}
	
	public static Document domFromString(String XMLString) throws ParserConfigurationException, SAXException, IOException {
		DocumentBuilder builder = getBuilder();
		InputStream str_stream = null;
		Document dom = null;
		str_stream = new ByteArrayInputStream(XMLString.getBytes("UTF-8"));
		try {
			dom = builder.parse(str_stream);
		} catch (IOException e) {
			throw e;
		} catch (SAXException e) {
			throw e;
		} finally {
			str_stream.close();
		}
		return dom;
	}
	
	public static void parseWithXPath(Document doc, String path, HashMap<String, String> resultMap, boolean depth) throws XPathExpressionException {
		XPathExpression expr = getXPathExpression(path);
		NodeList subTree = (NodeList) expr.evaluate(doc, XPathConstants.NODESET);
		parseSubTree(subTree, resultMap, depth);
	}
	
	public static void parseSubTree(NodeList subTree, HashMap<String, String> resultMap, boolean depth) {
		for (int i = 0; i < subTree.getLength(); i++) {
			Node currentNode = subTree.item(i);
			if (currentNode.getNodeType() == Node.ELEMENT_NODE) {
				String name = currentNode.getNodeName();
				String value = currentNode.getTextContent();
				resultMap.put(name, value);
				NodeList st = currentNode.getChildNodes();
				if (st.getLength() != 0 && depth){
					parseSubTree(st, resultMap, depth);
				}
			}
		}
	}
	
	public static String documentToString(Document doc) {
		StringWriter sw = new StringWriter();
        TransformerFactory tf = TransformerFactory.newInstance();
        Transformer transformer;
		try {
			transformer = tf.newTransformer();
		
	        transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "no");
	        transformer.setOutputProperty(OutputKeys.METHOD, "xml");
	        transformer.setOutputProperty(OutputKeys.INDENT, "yes");
	        transformer.setOutputProperty(OutputKeys.ENCODING, "UTF-8");
	
			transformer.transform(new DOMSource(doc), new StreamResult(sw));
			   
		} catch (TransformerConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (TransformerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return sw.toString();
	}
	
	public static Map<String, String> parseResponse(String XMLResponse, String xPath) throws ParserConfigurationException, SAXException, IOException, XPathExpressionException {
		HashMap<String, String> resultMap = new HashMap<String, String>();
		InputStream is = null;
		try {
			is = new ByteArrayInputStream(XMLResponse.getBytes("UTF-8"));
			DocumentBuilder builder = getBuilder();
			Document dom = builder.parse(is);
			parseWithXPath(dom, xPath, resultMap, false);
		} finally {
			is.close();
		}
		return resultMap;
	}
	
	public static Node findFirstByName(NodeList nodeList, String name) {
		for (int i = 0; i < nodeList.getLength(); ++i) {
			Node currentNode = nodeList.item(i);
			String nodeName = currentNode.getNodeName();
			if (nodeName.equals(name))
				return currentNode;
		}
		return null;
	}

	/**
	 * —А–∞–Ј–±–Њ—А –Њ—В–≤–µ—В–∞ –љ–∞ –Ј–∞–њ—А–Њ—Б –Є–љ—Д–Њ—А–Љ–∞—Ж–Є–Є –Њ –њ–Њ—Б–ї–µ–і–љ–µ–Љ –Њ–±–љ–Њ–≤–ї–µ–љ–Є–Є
	 * 
	 * @param XMLResponse
	 * @return
	 * @throws IOException
	 * @throws ParserConfigurationException
	 * @throws SAXException
	 */
	public static HashMap<String, String> parseLastUpdateInfoResponse(
			String XMLResponse) throws IOException,
			ParserConfigurationException, SAXException {
		HashMap<String, String> map = new HashMap<String, String>();
		try {
			Document dom =domFromString(XMLResponse);
			NodeList domNodes = dom.getChildNodes();
			Node response = domNodes.item(0);
			NodeList responseNodes = response.getChildNodes();
			String status = responseNodes.item(0).getFirstChild()
					.getNodeValue();
			if (status.equals(OK)) {
				NodeList info = responseNodes.item(1).getChildNodes();
				map.put(info.item(0).getNodeName(), info.item(0)
						.getFirstChild().getNodeValue());
				map.put(info.item(1).getNodeName(), info.item(1)
						.getFirstChild().getNodeValue());
			} else {
				return null;
			}
		} catch (UnsupportedEncodingException e) {
			throw e;
		} catch (ParserConfigurationException e) {
			e.printStackTrace();
			throw e;
		} catch (SAXException e) {
			e.printStackTrace();
			throw e;
		} 

		return map;
	}


	/**
	 * –њ–µ—А–µ–≤–Њ–і –Є–Ј —Б—В—А–Њ–Ї–Њ–≤–Њ–≥–Њ –Ј–љ–∞—З–µ–љ–Є—П –і–љ—П –љ–µ–і–µ–ї–Є –≤ –Є–љ—В–Њ–≤—Л–є
	 * 
	 * @param dow
	 * @return
	 */
	public static int getDayOfWeekInteger(String dow) {
		if (dow.equals("Sun"))
			return Calendar.SUNDAY;
		if (dow.equals("Mon"))
			return Calendar.MONDAY;
		if (dow.equals("Tue"))
			return Calendar.TUESDAY;
		if (dow.equals("Wed"))
			return Calendar.WEDNESDAY;
		if (dow.equals("Thu"))
			return Calendar.THURSDAY;
		if (dow.equals("Fri"))
			return Calendar.FRIDAY;
		if (dow.equals("Sat"))
			return Calendar.SATURDAY;

		return -1;
	}

}
