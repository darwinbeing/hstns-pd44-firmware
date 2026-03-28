// Derived from the Python version of SVD-Loader-Ghidra:
// https://github.com/leveldown-security/SVD-Loader-Ghidra
// Load specified SVD and generate peripheral memory maps & structures.
//@author Thomas Roth <thomas.roth@leveldown.de>, Ryan Pavlik <ryan.pavlik@gmail.com>
//@category leveldown security
//@keybinding
//@menupath
//@toolbar

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.data.ByteDataType;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeConflictHandler;
import ghidra.program.model.data.StructureDataType;
import ghidra.program.model.data.UnsignedIntegerDataType;
import ghidra.program.model.data.UnsignedLongLongDataType;
import ghidra.program.model.data.UnsignedShortDataType;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.mem.MemoryConflictException;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.SourceType;
import ghidra.program.model.symbol.SymbolTable;

public class SVDLoader extends GhidraScript {

	private static final class CpuInfo {
		String name;
		String endian;
	}

	private static final class AddressBlock {
		long offset;
		long size;
	}

	private static final class RegisterDef {
		String name;
		String description;
		long addressOffset;
		Integer sizeBits;

		RegisterDef copy() {
			RegisterDef copy = new RegisterDef();
			copy.name = name;
			copy.description = description;
			copy.addressOffset = addressOffset;
			copy.sizeBits = sizeBits;
			return copy;
		}
	}

	private static final class PeripheralDef {
		String name;
		String description;
		long baseAddress;
		Integer defaultRegisterSizeBits;
		AddressBlock addressBlock;
		List<RegisterDef> registers = new ArrayList<RegisterDef>();

		PeripheralDef copy() {
			PeripheralDef copy = new PeripheralDef();
			copy.name = name;
			copy.description = description;
			copy.baseAddress = baseAddress;
			copy.defaultRegisterSizeBits = defaultRegisterSizeBits;
			if (addressBlock != null) {
				copy.addressBlock = new AddressBlock();
				copy.addressBlock.offset = addressBlock.offset;
				copy.addressBlock.size = addressBlock.size;
			}
			for (RegisterDef register : registers) {
				copy.registers.add(register.copy());
			}
			return copy;
		}
	}

	private static final class DeviceDef {
		CpuInfo cpu;
		Integer defaultRegisterSizeBits;
		List<PeripheralDef> peripherals = new ArrayList<PeripheralDef>();
	}

	private static final class MemoryRegion implements Comparable<MemoryRegion> {
		AddressSpace space;
		long start;
		long end;
		List<String> nameParts = new ArrayList<String>();

		MemoryRegion(AddressSpace space, String name, long start, long end) {
			this.space = space;
			this.start = start;
			this.end = end;
			if (name != null) {
				nameParts.add(name);
			}
		}

		String getName() {
			return String.join("_", nameParts);
		}

		long length() {
			return end - start;
		}

		boolean overlaps(MemoryRegion other) {
			if (!space.equals(other.space)) {
				return false;
			}
			return start < other.end && other.start < end;
		}

		void combineFrom(MemoryRegion other) {
			start = Math.min(start, other.start);
			end = Math.max(end, other.end);
			nameParts.addAll(other.nameParts);
		}

		@Override
		public int compareTo(MemoryRegion other) {
			int spaceCompare = Integer.compare(space.getSpaceID(), other.space.getSpaceID());
			if (spaceCompare != 0) {
				return spaceCompare;
			}
			return Long.compare(start, other.start);
		}

		@Override
		public String toString() {
			return getName() + "(" + space.getName() + ":" + toHex(start) + ":" + toHex(end) + ")";
		}
	}

	private static final class RegisterContext {
		Map<String, RegisterDef> templates = new LinkedHashMap<String, RegisterDef>();
	}

	private static final class SvdParser {
		private final Document document;
		private final Element root;
		private final Element peripheralsElement;
		private final Map<String, Element> peripheralNodes = new LinkedHashMap<String, Element>();
		private final Map<String, PeripheralDef> resolvedPeripherals = new HashMap<String, PeripheralDef>();
		private final DeviceDef device = new DeviceDef();

		SvdParser(File file) throws Exception {
			DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
			factory.setNamespaceAware(false);
			DocumentBuilder builder = factory.newDocumentBuilder();
			document = builder.parse(file);
			root = document.getDocumentElement();
			peripheralsElement = child(root, "peripherals");
			if (peripheralsElement != null) {
				for (Element peripheralElement : children(peripheralsElement, "peripheral")) {
					String name = text(peripheralElement, "name");
					if (name != null) {
						peripheralNodes.put(name, peripheralElement);
					}
				}
			}
		}

		DeviceDef parse() {
			device.defaultRegisterSizeBits = intValue(root, "size");
			device.cpu = parseCpu();
			for (String name : peripheralNodes.keySet()) {
				device.peripherals.add(resolvePeripheral(name));
			}
			return device;
		}

		private CpuInfo parseCpu() {
			Element cpuElement = child(root, "cpu");
			if (cpuElement == null) {
				return null;
			}
			CpuInfo cpu = new CpuInfo();
			cpu.name = text(cpuElement, "name");
			cpu.endian = text(cpuElement, "endian");
			return cpu;
		}

		private PeripheralDef resolvePeripheral(String name) {
			if (resolvedPeripherals.containsKey(name)) {
				return resolvedPeripherals.get(name);
			}

			Element peripheralElement = peripheralNodes.get(name);
			if (peripheralElement == null) {
				throw new IllegalArgumentException("Unable to resolve peripheral: " + name);
			}

			String derivedFrom = derivedFrom(peripheralElement);
			PeripheralDef peripheral = derivedFrom != null ? resolvePeripheral(derivedFrom).copy() : new PeripheralDef();

			String explicitName = text(peripheralElement, "name");
			peripheral.name = explicitName != null ? explicitName : name;
			String description = text(peripheralElement, "description");
			if (description != null) {
				peripheral.description = description;
			}

			Long baseAddress = longValue(peripheralElement, "baseAddress");
			if (baseAddress != null) {
				peripheral.baseAddress = baseAddress.longValue();
			}

			Integer size = intValue(peripheralElement, "size");
			if (size != null) {
				peripheral.defaultRegisterSizeBits = size;
			}
			else if (peripheral.defaultRegisterSizeBits == null) {
				peripheral.defaultRegisterSizeBits = device.defaultRegisterSizeBits;
			}

			AddressBlock addressBlock = parseAddressBlock(peripheralElement);
			if (addressBlock != null) {
				peripheral.addressBlock = addressBlock;
			}

			Element registersElement = child(peripheralElement, "registers");
			if (registersElement != null) {
				RegisterContext context = new RegisterContext();
				peripheral.registers = parseRegistersContainer(registersElement, 0, "", peripheral.defaultRegisterSizeBits, context);
			}

			resolvedPeripherals.put(name, peripheral);
			return peripheral;
		}

		private AddressBlock parseAddressBlock(Element peripheralElement) {
			Element addressBlockElement = child(peripheralElement, "addressBlock");
			if (addressBlockElement == null) {
				return null;
			}
			AddressBlock block = new AddressBlock();
			Long offset = longValue(addressBlockElement, "offset");
			Long size = longValue(addressBlockElement, "size");
			block.offset = offset != null ? offset.longValue() : 0L;
			block.size = size != null ? size.longValue() : 0L;
			return block;
		}

		private List<RegisterDef> parseRegistersContainer(Element parent, long baseOffset, String prefix,
				Integer inheritedSizeBits, RegisterContext context) {
			List<RegisterDef> registers = new ArrayList<RegisterDef>();
			for (Element childElement : childElements(parent)) {
				String tag = childElement.getTagName();
				if ("register".equals(tag)) {
					registers.addAll(parseRegisterElement(childElement, baseOffset, prefix, inheritedSizeBits, context));
				}
				else if ("cluster".equals(tag)) {
					registers.addAll(parseClusterElement(childElement, baseOffset, prefix, inheritedSizeBits));
				}
			}
			return registers;
		}

		private List<RegisterDef> parseRegisterElement(Element registerElement, long baseOffset, String prefix,
				Integer inheritedSizeBits, RegisterContext context) {
			String rawName = text(registerElement, "name");
			String derivedFrom = derivedFrom(registerElement);
			RegisterDef baseRegister = derivedFrom != null ? context.templates.get(derivedFrom) : null;

			String nameTemplate = rawName != null ? rawName : (baseRegister != null ? baseRegister.name : null);
			if (nameTemplate == null) {
				return Collections.emptyList();
			}

			String description = text(registerElement, "description");
			if (description == null && baseRegister != null) {
				description = baseRegister.description;
			}

			Long localOffset = longValue(registerElement, "addressOffset");
			long resolvedOffset = localOffset != null ? localOffset.longValue() : (baseRegister != null ? baseRegister.addressOffset : 0L);

			Integer sizeBits = intValue(registerElement, "size");
			if (sizeBits == null && baseRegister != null) {
				sizeBits = baseRegister.sizeBits;
			}
			if (sizeBits == null) {
				sizeBits = inheritedSizeBits;
			}

			Integer dim = intValue(registerElement, "dim");
			if (dim == null) {
				RegisterDef register = new RegisterDef();
				register.name = prefix + nameTemplate;
				register.description = description;
				register.addressOffset = baseOffset + resolvedOffset;
				register.sizeBits = sizeBits;
				context.templates.put(nameTemplate, register.copy());
				return Collections.singletonList(register);
			}

			long dimIncrement = valueOr(longValue(registerElement, "dimIncrement"), sizeBits != null ? sizeBits.intValue() / 8 : 4);
			List<String> indices = parseDimIndices(registerElement, dim.intValue());
			List<RegisterDef> registers = new ArrayList<RegisterDef>();
			for (int i = 0; i < dim.intValue(); i++) {
				String index = i < indices.size() ? indices.get(i) : Integer.toString(i);
				RegisterDef register = new RegisterDef();
				register.name = prefix + formatDimName(nameTemplate, index);
				register.description = description;
				register.addressOffset = baseOffset + resolvedOffset + (i * dimIncrement);
				register.sizeBits = sizeBits;
				registers.add(register);
			}

			RegisterDef template = new RegisterDef();
			template.name = nameTemplate;
			template.description = description;
			template.addressOffset = resolvedOffset;
			template.sizeBits = sizeBits;
			context.templates.put(nameTemplate, template);

			return registers;
		}

		private List<RegisterDef> parseClusterElement(Element clusterElement, long baseOffset, String prefix,
				Integer inheritedSizeBits) {
			String clusterName = text(clusterElement, "name");
			if (clusterName == null) {
				return Collections.emptyList();
			}

			Long localOffset = longValue(clusterElement, "addressOffset");
			long resolvedOffset = localOffset != null ? localOffset.longValue() : 0L;

			Integer clusterSize = intValue(clusterElement, "size");
			Integer effectiveSizeBits = clusterSize != null ? clusterSize : inheritedSizeBits;

			Integer dim = intValue(clusterElement, "dim");
			long dimIncrement = valueOr(longValue(clusterElement, "dimIncrement"), 0L);
			List<String> indices = dim != null ? parseDimIndices(clusterElement, dim.intValue()) : Collections.<String>emptyList();

			List<RegisterDef> registers = new ArrayList<RegisterDef>();
			if (dim == null) {
				RegisterContext context = new RegisterContext();
				registers.addAll(parseRegistersContainer(clusterElement, baseOffset + resolvedOffset,
					prefix + clusterName + "_", effectiveSizeBits, context));
				return registers;
			}

			for (int i = 0; i < dim.intValue(); i++) {
				String index = i < indices.size() ? indices.get(i) : Integer.toString(i);
				String expandedPrefix = prefix + formatDimName(clusterName, index) + "_";
				RegisterContext context = new RegisterContext();
				registers.addAll(parseRegistersContainer(clusterElement,
					baseOffset + resolvedOffset + (i * dimIncrement), expandedPrefix, effectiveSizeBits, context));
			}
			return registers;
		}

		private List<String> parseDimIndices(Element element, int dim) {
			String dimIndexText = text(element, "dimIndex");
			if (dimIndexText == null || dimIndexText.trim().isEmpty()) {
				List<String> indices = new ArrayList<String>();
				for (int i = 0; i < dim; i++) {
					indices.add(Integer.toString(i));
				}
				return indices;
			}

			if (dimIndexText.indexOf(',') >= 0) {
				List<String> indices = new ArrayList<String>();
				for (String part : dimIndexText.split(",")) {
					indices.add(part.trim());
				}
				return indices;
			}

			if (dimIndexText.indexOf('-') >= 0) {
				String[] parts = dimIndexText.split("-", 2);
				int start = Integer.parseInt(parts[0].trim());
				int end = Integer.parseInt(parts[1].trim());
				List<String> indices = new ArrayList<String>();
				for (int i = start; i <= end; i++) {
					indices.add(Integer.toString(i));
				}
				return indices;
			}

			List<String> indices = new ArrayList<String>();
			indices.add(dimIndexText.trim());
			return indices;
		}

		private static String derivedFrom(Element element) {
			String attr = element.getAttribute("derivedFrom");
			if (attr != null && !attr.isEmpty()) {
				return attr;
			}
			return text(element, "derivedFrom");
		}

		private static String text(Element parent, String tag) {
			Element child = child(parent, tag);
			if (child == null) {
				return null;
			}
			String value = child.getTextContent();
			return value != null ? value.trim() : null;
		}

		private static Integer intValue(Element parent, String tag) {
			Long value = longValue(parent, tag);
			return value != null ? Integer.valueOf((int) value.longValue()) : null;
		}

		private static Long longValue(Element parent, String tag) {
			String text = text(parent, tag);
			if (text == null) {
				return null;
			}
			return parseLong(text);
		}

		private static Long parseLong(String text) {
			String value = text.trim().toLowerCase();
			try {
				if (value.startsWith("0x")) {
					return Long.valueOf(Long.parseLong(value.substring(2), 16));
				}
				if (value.startsWith("#")) {
					String binary = value.substring(1).replace('x', '0');
					boolean isBinary = true;
					for (int i = 0; i < binary.length(); i++) {
						char c = binary.charAt(i);
						if (c != '0' && c != '1') {
							isBinary = false;
							break;
						}
					}
					if (isBinary) {
						return Long.valueOf(Long.parseLong(binary, 2));
					}
					return Long.valueOf(Long.parseLong(binary));
				}
				if ("true".equals(value)) {
					return Long.valueOf(1);
				}
				if ("false".equals(value)) {
					return Long.valueOf(0);
				}
				return Long.valueOf(Long.parseLong(value));
			}
			catch (NumberFormatException e) {
				return null;
			}
		}

		private static Element child(Element parent, String tag) {
			for (Element child : childElements(parent)) {
				if (tag.equals(child.getTagName())) {
					return child;
				}
			}
			return null;
		}

		private static List<Element> children(Element parent, String tag) {
			List<Element> elements = new ArrayList<Element>();
			for (Element child : childElements(parent)) {
				if (tag.equals(child.getTagName())) {
					elements.add(child);
				}
			}
			return elements;
		}

		private static List<Element> childElements(Element parent) {
			List<Element> elements = new ArrayList<Element>();
			NodeList nodes = parent.getChildNodes();
			for (int i = 0; i < nodes.getLength(); i++) {
				Node node = nodes.item(i);
				if (node.getNodeType() == Node.ELEMENT_NODE) {
					elements.add((Element) node);
				}
			}
			return elements;
		}
	}

	@Override
	public void run() throws Exception {
		File svdFile = askFile("Choose SVD file", "Load SVD File");

		println("Loading SVD file...");
		SvdParser parser = new SvdParser(svdFile);
		DeviceDef device = parser.parse();
		println("\tDone!");

		String cpuType = device.cpu != null ? device.cpu.name : null;
		String cpuEndian = device.cpu != null ? device.cpu.endian : null;
		int defaultRegisterSizeBits = device.defaultRegisterSizeBits != null ? device.defaultRegisterSizeBits.intValue() : 32;

		if (cpuType != null && !cpuType.startsWith("CM")) {
			println("Currently only Cortex-M CPUs are supported, so this might not work...");
			println("Supplied CPU type was: " + cpuType);
		}

		if (cpuEndian != null && !"little".equals(cpuEndian)) {
			printerr("Currently only little endian CPUs are supported.");
			printerr("Supplied CPU endian was: " + cpuEndian);
			return;
		}

		Listing listing = currentProgram.getListing();
		SymbolTable symbolTable = currentProgram.getSymbolTable();
		AddressSpace codeSpace = currentProgram.getAddressFactory().getDefaultAddressSpace();
		AddressSpace dataSpace = getDataAddressSpace();
		Memory memory = currentProgram.getMemory();

		Namespace namespace = symbolTable.getNamespace("Peripherals", null);
		if (namespace == null) {
			namespace = symbolTable.createNameSpace(null, "Peripherals", SourceType.ANALYSIS);
		}

		println("Generating memory regions...");
		List<MemoryRegion> memoryRegions = new ArrayList<MemoryRegion>();
		for (PeripheralDef peripheral : device.peripherals) {
			if (!shouldCreateMemoryBlock(peripheral)) {
				continue;
			}
			AddressSpace peripheralSpace = getAddressSpaceForPeripheral(peripheral, codeSpace, dataSpace);
			long start = peripheral.baseAddress;
			long length = peripheral.addressBlock != null
				? peripheral.addressBlock.offset + peripheral.addressBlock.size
				: calculatePeripheralSize(peripheral, defaultRegisterSizeBits);
			long end = start + length;
			if (length > 0) {
				memoryRegions.add(new MemoryRegion(peripheralSpace, peripheral.name, start, end));
			}
		}
		memoryRegions = reduceMemoryRegions(memoryRegions);

		println("Generating memory blocks...");
		for (MemoryRegion region : memoryRegions) {
			println("\t" + region.toString());
			try {
				Address address = region.space.getAddress(region.start);
				MemoryBlock block = memory.createUninitializedBlock(region.getName(), address, region.length(), false);
				block.setRead(true);
				block.setWrite(true);
				block.setExecute(false);
				block.setVolatile(true);
				block.setComment("Generated by SVDLoader.");
			}
			catch (MemoryConflictException e) {
				println("\tFailed to generate due to conflict in memory block for: " + region.getName());
				println("\t" + e.getMessage());
			}
			catch (Exception e) {
				println("\tFailed to generate memory block for: " + region.getName());
				println("\t" + e.getMessage());
			}
		}
		println("\tDone!");

		println("Generating peripherals...");
		for (PeripheralDef peripheral : device.peripherals) {
			println("\t" + peripheral.name);
			if (peripheral.registers.isEmpty()) {
				println("\t\tNo registers.");
				continue;
			}

			long length = calculatePeripheralSize(peripheral, defaultRegisterSizeBits);
			StructureDataType peripheralStruct = new StructureDataType(peripheral.name, (int) length);
			long peripheralStart = peripheral.baseAddress;
			long peripheralEnd = peripheralStart + length;
			println("\t\t" + toHex(peripheralStart) + ":" + toHex(peripheralEnd));

			for (RegisterDef register : peripheral.registers) {
				int registerSizeBits = register.sizeBits != null ? register.sizeBits.intValue() : defaultRegisterSizeBits;
				int registerSizeBytes = registerSizeBits / 8;
				DataType registerType = new UnsignedIntegerDataType();
				if (registerSizeBytes == 1) {
					registerType = new ByteDataType();
				}
				else if (registerSizeBytes == 2) {
					registerType = new UnsignedShortDataType();
				}
				else if (registerSizeBytes == 8) {
					registerType = new UnsignedLongLongDataType();
				}

				println("\t\t\t" + register.name + "(" + toHex(register.addressOffset) + ":" +
					toHex(register.addressOffset + registerSizeBytes) + ")");
				peripheralStruct.replaceAtOffset((int) register.addressOffset, registerType, registerSizeBytes,
					register.name, register.description);
			}

			AddressSpace peripheralSpace = getAddressSpaceForPeripheral(peripheral, codeSpace, dataSpace);
			Address address = peripheralSpace.getAddress(peripheralStart);
			Address endAddress = address.add(length - 1);
			currentProgram.getDataTypeManager().addDataType(peripheralStruct, DataTypeConflictHandler.REPLACE_HANDLER);
			try {
				symbolTable.createLabel(address, peripheral.name, namespace, SourceType.USER_DEFINED);
			}
			catch (Exception e) {
				println("\t\tFailed to create label for " + peripheral.name + ": " + e.getMessage());
			}
			try {
				listing.clearCodeUnits(address, endAddress, false);
				listing.createData(address, peripheralStruct);
			}
			catch (Exception e) {
				println("\t\tFailed to generate peripheral " + peripheral.name + ": " + e.getMessage());
			}
		}
	}

	private static long calculatePeripheralSize(PeripheralDef peripheral, int defaultRegisterSizeBits) {
		long size = 0;
		for (RegisterDef register : peripheral.registers) {
			int registerSizeBits = register.sizeBits != null ? register.sizeBits.intValue() : defaultRegisterSizeBits;
			size = Math.max(size, register.addressOffset + (registerSizeBits / 8));
		}
		return size;
	}

	private static List<MemoryRegion> reduceMemoryRegions(List<MemoryRegion> regions) {
		if (regions.isEmpty()) {
			return regions;
		}
		Collections.sort(regions);
		List<MemoryRegion> result = new ArrayList<MemoryRegion>();
		result.add(regions.get(0));
		for (int i = 1; i < regions.size(); i++) {
			MemoryRegion region = regions.get(i);
			MemoryRegion previous = result.get(result.size() - 1);
			if (region.overlaps(previous)) {
				previous.combineFrom(region);
			}
			else {
				result.add(region);
			}
		}
		return result;
	}

	private static String formatDimName(String template, String index) {
		if (template.indexOf("%s") >= 0) {
			return template.replace("%s", index);
		}
		return template + index;
	}

	private static long valueOr(Long value, long fallback) {
		return value != null ? value.longValue() : fallback;
	}

	private static String toHex(long value) {
		return "0x" + Long.toHexString(value);
	}

	private AddressSpace getDataAddressSpace() {
		AddressSpace space = currentProgram.getAddressFactory().getAddressSpace("ram");
		if (space == null) {
			space = currentProgram.getAddressFactory().getAddressSpace("RAM");
		}
		if (space == null) {
			space = currentProgram.getAddressFactory().getAddressSpace("DATA");
		}
		if (space == null) {
			space = currentProgram.getAddressFactory().getAddressSpace("data");
		}
		if (space == null) {
			space = currentProgram.getAddressFactory().getDefaultAddressSpace();
		}
		return space;
	}

	private boolean shouldCreateMemoryBlock(PeripheralDef peripheral) {
		String name = peripheral.name != null ? peripheral.name.toUpperCase() : "";
		if ("RESET".equals(name) || "IVT".equals(name) || "AIVT".equals(name) ||
			"RESERVED".equals(name) || "PROGRAM".equals(name)) {
			return false;
		}
		if ("SFR_SPACE".equals(name) || "X_DATA_RAM".equals(name) || "Y_DATA_RAM".equals(name) ||
			"DMA_RAM".equals(name) || "UNIMPLEMENTED".equals(name) || "PROGRAM_MAPPED_SPACE".equals(name)) {
			return false;
		}
		if ("DEVID".equals(name) || "__DEVID".equals(name)) {
			return false;
		}
		return true;
	}

	private AddressSpace getAddressSpaceForPeripheral(PeripheralDef peripheral, AddressSpace codeSpace, AddressSpace dataSpace) {
		String name = peripheral.name != null ? peripheral.name.toUpperCase() : "";
		if ("CONFIG".equals(name) || "DEVID".equals(name) || "__DEVID".equals(name)) {
			return codeSpace;
		}
		if ("SFR_SPACE".equals(name) || "X_DATA_RAM".equals(name) || "Y_DATA_RAM".equals(name) ||
			"DMA_RAM".equals(name) || "UNIMPLEMENTED".equals(name) || "PROGRAM_MAPPED_SPACE".equals(name)) {
			return dataSpace;
		}
		if ("DATA".equals(name) || "SFR".equals(name)) {
			return dataSpace;
		}
		if (!peripheral.registers.isEmpty()) {
			return dataSpace;
		}
		return codeSpace;
	}
}
