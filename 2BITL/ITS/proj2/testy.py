# -*- coding: utf-8 -*-
#login: xpavli78
#jmeno: Jan Pavlica
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
import unittest, time, re

#v ramci SetUp faze je v kazdem testu prihlasen pozadovany uzivatele a 
#jsou vytvorena vsechny pomocne polozky (uzivatele,skupiny)

#v ramci TearDown faze jsou vsechny pomocne polozky smazany

#testovani pridani skupiny
class t01_AddGroup(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_01_a_add(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        time.sleep(2)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00TESTGROUP_78")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00testgroup_78"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00testgroup_78", driver.find_element_by_link_text("00testgroup_78").text)
        
    def test_02_b_add_another(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        time.sleep(2)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00TESTGROUP_78")
        driver.find_element_by_name("add_and_add_another").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "span.fa.fa-check-circle-o"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("User Group successfully added", driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div/div").text)
        driver.find_element_by_css_selector("button.close").click()
        
    def test_03_c_add_and_edit(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        time.sleep(2)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00TESTGROUP_78")
        driver.find_element_by_name("add_and_edit").click()
        for i in range(15):
            try:
                if self.is_element_present(By.XPATH, "//div[@id='notification']/div[2]/div"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        # Warning: assertTextPresent may require manual changes
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*User Group successfully added[\s\S]*$")
        self.assertEqual("00testgroup_78", driver.find_element_by_css_selector("span.facet-pkey").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00testgroup_78"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00testgroup_78[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00testgroup_78']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani smazani skupiny
class t02_Del(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPTODELETE")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00grouptodelete"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00grouptodelete", driver.find_element_by_link_text("00grouptodelete").text)
        
    
    def test_04_a_del(self):
        driver = self.driver
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00grouptodelete[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00grouptodelete']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00grouptodelete[\s\S]*$")
        
    def test_05_b_del_settings(self):
        driver = self.driver
        driver.find_element_by_link_text("00grouptodelete").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Settings"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("Settings").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Actions"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("Actions").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Delete"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(4)
        driver.find_element_by_link_text("Delete").click()
        time.sleep(4)
        driver.find_element_by_name("ok").click()
        for i in range(15):
            try:
                if "^[\\s\\S]*Deleted group \"00grouptodelete\"[\\s\\S]*$": break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani pridani uzivatele do skupiny
class t03_AddUserToGroup(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get("https://ipa.demo1.freeipa.org/ipa/ui/#/e/user/search")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        driver.find_element_by_name("add").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "label[name=\"uid\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00test_user_for_adding_to_group")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[2]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[2]/div[2]/div/div/input").send_keys("Test")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[3]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[3]/div[2]/div/div/input").send_keys("User")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_adding_to_group[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.get(self.base_url)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("add").click()
        time.sleep(5)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORADDING")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforadding", driver.find_element_by_link_text("00groupforadding").text)
        
    def test_06_adding_user(self):
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("00groupforadding").click()
        for i in range(15):
            try:
                if "00groupforadding" == driver.find_element_by_css_selector("span.facet-pkey").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "add"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("add").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_adding_to_group[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00test_user_for_adding_to_group']").click()
        driver.find_element_by_css_selector("div.col-sm-12.col-xs-6 > button[name=\"add\"]").click()
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_adding_to_group@demo1\.freeipa\.org[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        self.assertEqual("00test_user_for_adding_to_group", driver.find_element_by_link_text("00test_user_for_adding_to_group").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforadding']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(3)
        driver.get("https://ipa.demo1.freeipa.org/ipa/ui/#/e/user/search")
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00test_user_for_adding_to_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(1)
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00test_user_for_adding_to_group[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00test_user_for_adding_to_group']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(3)
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani odstraneni uzivatele ze skupiny
class t04_RemoveUserFromGroup(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get("https://ipa.demo1.freeipa.org/ipa/ui/#/e/user/search")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        driver.find_element_by_name("add").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "label[name=\"uid\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00test_user_for_removing")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[2]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[2]/div[2]/div/div/input").send_keys("Test")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[3]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[3]/div[2]/div/div/input").send_keys("User")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_removing[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.get(self.base_url)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        time.sleep(4)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORREMOVING")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforremoving"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforremoving", driver.find_element_by_link_text("00groupforremoving").text)
        driver.find_element_by_link_text("00groupforremoving").click()
        for i in range(15):
            try:
                if "00groupforremoving" == driver.find_element_by_css_selector("span.facet-pkey").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "add"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("add").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_removing[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00test_user_for_removing']").click()
        driver.find_element_by_css_selector("div.col-sm-12.col-xs-6 > button[name=\"add\"]").click()
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00test_user_for_removing@demo1\.freeipa\.org[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        self.assertEqual("00test_user_for_removing", driver.find_element_by_link_text("00test_user_for_removing").text)
        
    def test_07_remove_user(self):
        driver = self.driver 
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00test_user_for_removing"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00test_user_for_removing']").click()
        driver.find_element_by_name("remove").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*Remove Users from User Group 00groupforremoving[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*No entries\.[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("No entries.", driver.find_element_by_css_selector("div.summary").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforremoving"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforremoving[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforremoving']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(10)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforremoving[\s\S]*$")
        driver.get("https://ipa.demo1.freeipa.org/ipa/ui/#/e/user/search")
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00test_user_for_removing"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.slee(15)
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00test_user_for_removing[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00test_user_for_removing']").click()
        driver.find_element_by_name("remove").click()
        time.sleep(5)
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(10)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00test_user_for_removing[\s\S]*$")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani pridani skupiny do skupiny
class t05_AddGroupToGroup(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORADDING")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforadding", driver.find_element_by_link_text("00groupforadding").text)
        driver.get(self.base_url)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORADDING2")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforadding2", driver.find_element_by_link_text("00groupforadding2").text)
        
    def test_08_adding_group(self):
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("00groupforadding").click()
        for i in range(15):
            try:
                if "00groupforadding" == driver.find_element_by_css_selector("span.facet-pkey").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "member_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("member_group").click()
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "add"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("add").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00groupforadding2[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00groupforadding2']").click()
        driver.find_element_by_css_selector("div.col-sm-12.col-xs-6 > button[name=\"add\"]").click()
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00groupforadding2[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        self.assertEqual("00groupforadding2", driver.find_element_by_link_text("00groupforadding2").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforadding']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        time.sleep(4)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding2[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforadding2']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        time.sleep(4)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding[\s\S]*$")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani odstraneni skupiny ze skupiny
class t06_RemoveGroupFromGroup(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORADDING")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforadding", driver.find_element_by_link_text("00groupforadding").text)
        driver.get(self.base_url)
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("00GROUPFORADDING2")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("00groupforadding2", driver.find_element_by_link_text("00groupforadding2").text)
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("00groupforadding").click()
        for i in range(15):
            try:
                if "00groupforadding" == driver.find_element_by_css_selector("span.facet-pkey").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "member_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("member_group").click()
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "add"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        driver.find_element_by_name("add").click()
        # Warning: waitForTextPresent may require manual changes
        time.sleep(3)
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00groupforadding2[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00groupforadding2']").click()
        driver.find_element_by_css_selector("div.col-sm-12.col-xs-6 > button[name=\"add\"]").click()
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*00groupforadding2[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(2)
        self.assertEqual("00groupforadding2", driver.find_element_by_link_text("00groupforadding2").text)
        
    def test_09_remove_group(self):
        driver = self.driver
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='00groupforadding2']").click()
        driver.find_element_by_name("remove").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "ok"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*No entries\.[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("No entries.", driver.find_element_by_css_selector("div.summary").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforadding']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        time.sleep(4)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "00groupforadding2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*00groupforadding2[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='00groupforadding2']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        time.sleep(5)
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


#testovani editace informaci o skupine
class t07_Edit(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
        driver.find_element_by_name("add").click()
        time.sleep(4)
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("0GROUPFOREDITING")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0groupforediting"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("0groupforediting", driver.find_element_by_link_text("0groupforediting").text)
        
    def test_10_a_editing(self):
        driver = self.driver
        driver.find_element_by_link_text("0groupforediting").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Settings"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("Settings").click()
        time.sleep(5)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "description"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        driver.find_element_by_xpath("//div[@id='container']/div/div/div/div[2]/div/div/div/div/div[2]/div[2]/div/textarea").send_keys("Skupina byla editovana 78.")
        driver.find_element_by_name("save").click()
        time.sleep(5)
        for i in range(15):
            try:
                if self.is_element_present(By.XPATH, "//div[@id='notification']/div[2]/div"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        driver.find_element_by_link_text("User Groups").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0groupforediting"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(10)
        self.assertEqual("Skupina byla editovana 78.", driver.find_element_by_css_selector("div[name=\"description\"]").text)
    
    def test_12_c_editing_with_undo(self):
        driver = self.driver
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0groupforediting"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("0groupforediting").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Settings"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("Settings").click()
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "description"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        driver.find_element_by_xpath("//div[@id='container']/div/div/div/div[2]/div/div/div/div/div[2]/div[2]/div/textarea").send_keys("Skupina byla editovana 78.")
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div[name=\"description\"] > button[name=\"undo\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_css_selector("div[name=\"description\"] > button[name=\"undo\"]").click()
        
    def test_11_b_editing_with_revert(self):
        driver = self.driver
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0groupforediting"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("0groupforediting").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "Settings"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_link_text("Settings").click()
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "description"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        driver.find_element_by_xpath("//div[@id='container']/div/div/div/div[2]/div/div/div/div/div[2]/div[2]/div/textarea").send_keys("Skupina byla editovana 78.")
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "revert"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("revert").click()
        
        
        

    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0groupforediting"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_xpath("//input[@value='0groupforediting']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0groupforediting[\s\S]*$")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


#testovani vyhledavaciho pole
class t08_Search(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_13_search(self):
        driver = self.driver
        driver.find_element_by_name("filter").clear()
        driver.find_element_by_name("filter").send_keys("tor")
        driver.find_element_by_name("find").click()
        time.sleep(5)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "admins"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("admins", driver.find_element_by_link_text("admins").text)
        self.assertEqual("editors", driver.find_element_by_link_text("editors").text)
        self.assertEqual("trust admins", driver.find_element_by_link_text("trust admins").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


#testovani nevalidnich znaku ve jmenu skupiny
class t09_InvalidChar(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_14_invalid_char(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys(",,")
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div[name=\"cn\"] > span[name=\"error_link\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(1)
        self.assertEqual("may only include letters, numbers, _, -, . and $", driver.find_element_by_css_selector("div[name=\"cn\"] > span[name=\"error_link\"]").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


#testovani pridani skupiny se stejnym jmenem
class t10_AddGroupWithSameName(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_15_same_name(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("0SAME_NAME_GROUP")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0same_name_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("0same_name_group", driver.find_element_by_link_text("0same_name_group").text)
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("0SAME_NAME_GROUP")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div[name=\"cn\"] > span[name=\"error_link\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(3)
        driver.find_element_by_css_selector("#error_dialog > div.modal-content > div.modal-header > button.close").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "button.close"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(3)
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"cancel\"]").click()
        time.sleep(3)
        
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0same_name_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0same_name_group[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='0same_name_group']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(1)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0same_name_group[\s\S]*$")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani pridani skupiny se stejnym POSIX GID
class t11_AddGroupWithSameGid(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_16_same_gid(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("0SAME_GID_GROUP")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[4]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[4]/div[2]/div/div/input").send_keys("481995")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0same_gid_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("0same_gid_group", driver.find_element_by_link_text("0same_gid_group").text)
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("0SAME_GID_GROUP2")
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[4]/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div[4]/div[2]/div/div/input").send_keys("481995")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div[name=\"cn\"] > span[name=\"error_link\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_css_selector("#error_dialog > div.modal-content > div.modal-header > button.close").click()
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0same_gid_group"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0same_gid_group[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='0same_gid_group']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(1)
        for i in range(15):
            try:
                if self.is_element_present(By.LINK_TEXT, "0same_gid_group2"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        self.assertRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0same_gid_group2[\s\S]*$")
        driver.find_element_by_xpath("//input[@value='0same_gid_group2']").click()
        driver.find_element_by_name("remove").click()
        driver.find_element_by_name("ok").click()
        # Warning: waitForTextPresent may require manual changes
        for i in range(15):
            try:
                if re.search(r"^[\s\S]*1 item\(s\) deleted[\s\S]*$", driver.find_element_by_css_selector("BODY").text): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(1)
        self.assertNotRegexpMatches(driver.find_element_by_css_selector("BODY").text, r"^[\s\S]*0same_gid_group2[\s\S]*$")
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani pridani skupiny bez vyplneni jmena
class t12_AddGroupWithoutName(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("admin")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_17_add_group_without_name(self):
        driver = self.driver
        driver.find_element_by_name("add").click()
        time.sleep(5)
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div[name=\"cn\"] > span[name=\"error_link\"]"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        self.assertEqual("Required field", driver.find_element_by_css_selector("div[name=\"cn\"] > span[name=\"error_link\"]").text)
        driver.find_element_by_name("cancel").click()
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)


#testovani pridani skupiny bez prav (jako manager)
class t13_AddGroupAsManager(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("manager")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_18_add_as_manager(self):
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(5)
        self.assertFalse(driver.find_element_by_name("add").is_displayed())
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
        
        
#testovani pridani skupiny bez prav (jako employee)
class t14_AddGroupAsEmployee(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("employee")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_19_add_as_employee(self):
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("02TESTGROUP_78")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "#error_dialog > div.modal-content > div.modal-header > h4.modal-title"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(7)
        self.assertEqual("IPA Error 2100: ACIError", driver.find_element_by_css_selector("#error_dialog > div.modal-content > div.modal-header > h4.modal-title").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
                
                
#testovani pridani skupiny bez prav (jako helpdesk)
class t15_AddGroupAsHelpdesk(unittest.TestCase):
    def setUp(self):
        self.driver = webdriver.Remote(command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',desired_capabilities=DesiredCapabilities.FIREFOX)
        self.driver.implicitly_wait(30)
        self.base_url = "https://ipa.demo1.freeipa.org/ipa/ui/#/e/group/search"
        self.verificationErrors = []
        self.accept_next_alert = True
        driver = self.driver
        driver.get(self.base_url)
        for i in range(15):
            try:
                if self.is_element_present(By.NAME, "username"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("username").click()
        driver.find_element_by_name("username").send_keys("helpdesk")
        driver.find_element_by_name("password").click()
        driver.find_element_by_name("password").send_keys("Secret123")
        driver.find_element_by_xpath("//button[@name='login']").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "div.facet-title.no-pkey"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        #self.assertEqual("Administrator", driver.find_element_by_css_selector("span.loggedinas").text)
    
    def test_20_add_as_helpdesk(self):
        driver = self.driver
        for i in range(15):
            try:
                if "User Groups" == driver.find_element_by_css_selector("h1 > span").text: break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        driver.find_element_by_name("add").click()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").clear()
        driver.find_element_by_xpath("//div[@id='add']/div/div[2]/div[2]/div/div/div/div[2]/div/div/input").send_keys("03TESTGROUP_78")
        driver.find_element_by_css_selector("div.rcue-dialog-buttons > button[name=\"add\"]").click()
        for i in range(15):
            try:
                if self.is_element_present(By.CSS_SELECTOR, "#error_dialog > div.modal-content > div.modal-header > h4.modal-title"): break
            except: pass
            time.sleep(1)
        else: self.fail("time out")
        time.sleep(7)
        self.assertEqual("IPA Error 2100: ACIError", driver.find_element_by_css_selector("#error_dialog > div.modal-content > div.modal-header > h4.modal-title").text)
    
    def is_element_present(self, how, what):
        try: self.driver.find_element(by=how, value=what)
        except NoSuchElementException as e: return False
        return True
    
    def is_alert_present(self):
        try: self.driver.switch_to_alert()
        except NoAlertPresentException as e: return False
        return True
    
    def close_alert_and_get_its_text(self):
        try:
            alert = self.driver.switch_to_alert()
            alert_text = alert.text
            if self.accept_next_alert:
                alert.accept()
            else:
                alert.dismiss()
            return alert_text
        finally: self.accept_next_alert = True
    
    def tearDown(self):
        self.driver.quit()
        self.assertEqual([], self.verificationErrors)
                
                
if __name__ == "__main__":
    unittest.main()
